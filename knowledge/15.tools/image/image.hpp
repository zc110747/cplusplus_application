//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      image.hpp
//
//  Purpose:
//      图像处理接口
//
//  Author:
//     	@公众号：<嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <string>
#include <memory>
#include <filesystem>
#include <fstream>

namespace bmp_img
{

enum class bmp_compress_state
{
    IMAGE_NOCOMPRESS = 0,
    IMAGE_COMPRESS,
};

#define BMP_FILEHEADER_LEN  14
#define BMP_INFOHEADER_LEN  40
#define BMP_HEAD_LEN        (BMP_FILEHEADER_LEN + BMP_INFOHEADER_LEN)

#define BMP_FILE_TYPE      0x4d42

//调色板统一展开成 256 项(每项 4 字节), 便于 1/4/8bpp 统一按 8bpp 输出
#define BMP_PALETTE_FULL_LEN  (256 * 4)

#pragma pack(push, 1)
struct bmp_file_header
{
    uint16_t bfType;      //文件类型
    uint32_t bfSize;      //位图文件大小(此处不一定准确)
    uint16_t reserved0;
    uint16_t reserved1;
    uint32_t offsets;     //实际数据偏移值
};

struct bmp_info_header
{
    uint32_t bsize;           //信息头
    uint32_t bwidth;          //图像每行像素点
    uint32_t bheight;         //图像行数(负值表示 top-down)
    uint16_t bPlanes;         //颜色平面数(总为1)
    uint16_t bBitCount;       //说明比特数/像素(每个像素点占有bit数)
    uint32_t bCompression;    //数据压缩类型
    uint32_t bSizeImage;      //图像数据的大小
    uint32_t bXPelsPerMeter;  //水平分辨率
    uint32_t bYPelsPerMeter;  //垂直分辨率
    uint32_t bClrUsed;        //颜色索引数
    uint32_t bClrImportant;   //说明对图像有重要影响的颜色索引 
};
#pragma pack(pop)

class bmp_decoder
{
public:
    bmp_decoder() = default;

    //内部用 unique_ptr 持有缓冲区, 拷贝已不可用, 显式声明避免误导
    bmp_decoder(const bmp_decoder&) = delete;
    bmp_decoder& operator=(const bmp_decoder&) = delete;

    bool decode_bmp(const std::string& filename)
    {
        std::ifstream file(std::filesystem::path(filename), std::ios::binary);
        if (!file.is_open()) {
            std::cout << "File open failed!" << std::endl;
            return false;
        }

        //解析bmp文件头数据
        std::unique_ptr<uint8_t[]> bmp_header_buf = std::make_unique<uint8_t[]>(BMP_HEAD_LEN);

        file.read(reinterpret_cast<char *>(bmp_header_buf.get()), BMP_HEAD_LEN);
        if (file.gcount() != BMP_HEAD_LEN) {
            std::cout << "bmp header too short!" << std::endl;
            return false;
        }

        const bmp_file_header *file_header = reinterpret_cast<const bmp_file_header *>(bmp_header_buf.get());
        const bmp_info_header *info_header = reinterpret_cast<const bmp_info_header *>(bmp_header_buf.get() + BMP_FILEHEADER_LEN);

        if (file_header->bfType != BMP_FILE_TYPE) {
            std::cout << "not a bmp file!" << std::endl;
            return false;
        }

        //当前只支持未压缩位图
        if (info_header->bCompression != 0) {
            std::cout << "compressed bmp not support!" << std::endl;
            return false;
        }

        if (info_header->bwidth == 0 || info_header->bheight == 0) {
            std::cout << "invalid bmp size!" << std::endl;
            return false;
        }

        bits_per_pixel_ = info_header->bBitCount;
        if (bits_per_pixel_ != 1 && bits_per_pixel_ != 4 && bits_per_pixel_ != 8 &&
            bits_per_pixel_ != 24 && bits_per_pixel_ != 32) {
            std::cout << "unsupported bpp: " << bits_per_pixel_ << std::endl;
            return false;
        }

        width_ = info_header->bwidth;

        //高度为负表示 top-down 位图, 数据行不再倒序存放
        int32_t raw_height = static_cast<int32_t>(info_header->bheight);
        top_down_ = raw_height < 0;
        height_ = static_cast<uint32_t>(top_down_ ? -raw_height : raw_height);

        //1/4/8bpp 是调色板索引, 内部统一展开成每像素 1 字节
        bytes_per_pixel_ = (bits_per_pixel_ <= 8) ? 1 : bits_per_pixel_ / 8;
        color_depth_ = (bits_per_pixel_ <= 8) ? 8 : bits_per_pixel_;
        compress_ = info_header->bCompression;

        //BMP 每行数据按 4 字节对齐
        uint32_t row_bytes_in = ((width_ * bits_per_pixel_ + 31u) / 32u) * 4u;

        pattle_len_ = 0;
        palette_data_.reset();
        if (bits_per_pixel_ <= 8) {
            uint32_t pal_len = 0;
            if (file_header->offsets > BMP_HEAD_LEN) {
                pal_len = file_header->offsets - BMP_HEAD_LEN;
            } else if (info_header->bClrUsed != 0) {
                pal_len = info_header->bClrUsed * 4;
            } else {
                pal_len = (1u << bits_per_pixel_) * 4;
            }
            if (pal_len > BMP_PALETTE_FULL_LEN) {
                pal_len = BMP_PALETTE_FULL_LEN;
            }

            palette_data_ = std::make_unique<uint8_t[]>(BMP_PALETTE_FULL_LEN);
            std::memset(palette_data_.get(), 0, BMP_PALETTE_FULL_LEN);
            file.read(reinterpret_cast<char *>(palette_data_.get()), pal_len);
            if (file.gcount() != static_cast<std::streamsize>(pal_len)) {
                std::cout << "palette data truncated!" << std::endl;
                return false;
            }
            pattle_len_ = BMP_PALETTE_FULL_LEN;
        }

        image_data_size_ = static_cast<uint64_t>(width_) * height_ * bytes_per_pixel_;
        image_data_ = std::make_unique<uint8_t[]>(image_data_size_);

        std::streamsize raw_size = static_cast<std::streamsize>(row_bytes_in) * static_cast<std::streamsize>(height_);
        std::unique_ptr<uint8_t[]> raw_data = std::make_unique<uint8_t[]>(static_cast<size_t>(raw_size));
        file.read(reinterpret_cast<char *>(raw_data.get()), raw_size);
        if (file.gcount() != raw_size) {
            std::cout << "image data truncated!" << std::endl;
            image_data_.reset();
            image_data_size_ = 0;
            return false;
        }

        /*
            翻转图像内数据所在行: 默认位图按 bottom-up 存放, 内部统一成 top-down
        */ 
        for (uint32_t row = 0; row < height_; ++row) {
            uint32_t src_row = top_down_ ? row : (height_ - 1u - row);
            const uint8_t *p_src = raw_data.get() + static_cast<size_t>(src_row) * row_bytes_in;
            uint8_t *p_dst = image_data_.get() + static_cast<size_t>(row) * width_ * bytes_per_pixel_;
            convert_row(p_src, p_dst);
        }

        file.close();
        return true;
    }

    void show_info(void) const
    {
        std::cout<<"width: "<<width_<<std::endl;
        std::cout<<"height: "<<height_<<std::endl;
        std::cout<<"bits_per_pixel: "<<bits_per_pixel_<<std::endl;
        std::cout<<"color_depth: "<<color_depth_<<std::endl;
        std::cout<<"bytes_per_pixel: "<<bytes_per_pixel_<<std::endl;
        std::cout<<"image_data_size_: "<<image_data_size_<<std::endl;
        std::cout<<"compress: "<<compress_<<std::endl;
        std::cout<<"pattle_len_: "<<pattle_len_<<std::endl;
        std::cout<<"top_down: "<<(top_down_ ? 1 : 0)<<std::endl;
    }

    bool save_to_bmp(const std::string filename, bmp_compress_state compress = bmp_compress_state::IMAGE_NOCOMPRESS)
    {
        //先检查数据, 避免失败路径截断已有的同名文件
        if (image_data_ == nullptr || image_data_size_ == 0) {
            std::cout << "image_data_ is null!" << std::endl;
            return false; 
        }

        uint8_t my_compress = get_compress_value(compress);

        //输出时每行仍按 4 字节对齐, 不足补 0
        uint32_t row_bytes_out = ((width_ * bytes_per_pixel_ + 3u) / 4u) * 4u;
        uint32_t palette_len_out = (bytes_per_pixel_ == 1) ? pattle_len_ : 0;

        std::ofstream file(std::filesystem::path(filename), std::ios::binary | std::ios::trunc);
        if (!file.is_open()) {
            std::cout << "File open failed!" << std::endl;
            return false; 
        }

        //添加bmp文件头
        bmp_file_header file_header;
        bmp_info_header info_header;
        file_header.bfType = BMP_FILE_TYPE;
        file_header.reserved0 = 0;
        file_header.reserved1 = 0;
        file_header.offsets = BMP_HEAD_LEN + palette_len_out;

        //添加bmp信息头
        info_header.bsize = BMP_INFOHEADER_LEN;
        info_header.bwidth = width_;
        info_header.bheight = height_;
        info_header.bPlanes = 1;
        info_header.bBitCount = color_depth_;
        info_header.bCompression = my_compress;  //压缩方式
        info_header.bSizeImage = row_bytes_out * height_;
        info_header.bXPelsPerMeter = 0;
        info_header.bYPelsPerMeter = 0;
        info_header.bClrUsed = 0;
        info_header.bClrImportant = 0;

        //位图文件长度(包含图像数据信息，文件头和位图数据的总长度)
        file_header.bfSize = info_header.bSizeImage + file_header.offsets;

        std::unique_ptr<uint8_t[]> p_header = std::make_unique<uint8_t[]>(BMP_HEAD_LEN);
        memcpy(p_header.get(), (char *)&file_header, BMP_FILEHEADER_LEN);
        memcpy(p_header.get() + BMP_FILEHEADER_LEN, (char *)&info_header, BMP_INFOHEADER_LEN);
        file.write((char *)p_header.get(), BMP_HEAD_LEN);
        
        //调色板定义(每个色彩4字节), 仅 8bpp 索引位图需要
        if (palette_len_out != 0) {
            file.write((char *)palette_data_.get(), palette_len_out);
        }

        //按 bottom-up 顺序写出图像数据
        std::unique_ptr<uint8_t[]> row_buffer = std::make_unique<uint8_t[]>(row_bytes_out);
        for (uint32_t i = 0; i < height_; ++i) {
            uint32_t src_row = height_ - 1u - i;
            std::memset(row_buffer.get(), 0, row_bytes_out);
            std::memcpy(row_buffer.get(),
                        image_data_.get() + static_cast<size_t>(src_row) * width_ * bytes_per_pixel_,
                        static_cast<size_t>(width_) * bytes_per_pixel_);
            file.write((char *)row_buffer.get(), row_bytes_out);
        }

        file.close();
        return true;
    }

private:
    // 获取压缩方式, 压缩写盘尚未实现, 统一按不压缩处理
    uint8_t get_compress_value(bmp_compress_state compress) const
    {
        if (compress == bmp_compress_state::IMAGE_COMPRESS) {
            std::cout << "bmp compress not implement, use nocompress!" << std::endl;
        }
        return 0;
    }

    //按位深把一行原始数据转换成内部格式(索引值 1 字节, 或 24/32 位原样拷贝)
    void convert_row(const uint8_t *p_src, uint8_t *p_dst) const
    {
        if (bits_per_pixel_ > 8) {
            std::memcpy(p_dst, p_src, static_cast<size_t>(width_) * bytes_per_pixel_);
            return;
        }

        uint32_t pix_per_byte = 8u / bits_per_pixel_;
        uint32_t mask = (1u << bits_per_pixel_) - 1u;

        for (uint32_t x = 0; x < width_; ++x) {
            uint32_t byte = p_src[x / pix_per_byte];
            uint32_t shift = (pix_per_byte - 1u - (x % pix_per_byte)) * bits_per_pixel_;
            p_dst[x] = static_cast<uint8_t>((byte >> shift) & mask);
        }
    }

private:
    // 图像数据
    std::unique_ptr<uint8_t[]> image_data_{nullptr};

    std::unique_ptr<uint8_t[]> palette_data_{nullptr};

    uint32_t pattle_len_{0};

    // 图形宽度
    uint32_t width_{0};

    // 图形高度         
    uint32_t height_{0};

    //文件中每个像素点占的 bit 数
    uint16_t bits_per_pixel_{0};

    //内部每个像素点占的 Byte 数
    uint16_t bytes_per_pixel_{0};

    // 数据总数
    uint64_t image_data_size_{0};

    //图像的色深(内部格式: 8/24/32)
    uint16_t color_depth_{0};

    // 压缩方式
    uint32_t compress_{0};

    //是否为 top-down 位图
    bool top_down_{false};
};
}
