/*

*/

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
    uint32_t bheight;         //图像行数
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
    bmp_decoder(const bmp_decoder&) = default;

    bool decode_bmp(std::string filename)
    {
        std::filesystem::path p(filename);
        std::ifstream file(p, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "File open failed!" << std::endl;
            return false;
        }

        //解析bmp文件头数据
        std::unique_ptr<uint8_t[]> bmp_header_buf = std::make_unique<uint8_t[]>(BMP_HEAD_LEN);       
        bmp_file_header *file_header;
        bmp_info_header *info_header;
        uint32_t offsets;

        file.read((char *)bmp_header_buf.get(), BMP_HEAD_LEN);
        file_header = (bmp_file_header *)bmp_header_buf.get();
        info_header = (bmp_info_header *)(bmp_header_buf.get()+14);
        offsets = file_header->offsets;

        //清理除头部信息外的额外bmp信息
        if (offsets > BMP_HEAD_LEN) {
            pattle_len_ = offsets - BMP_HEAD_LEN;
            palette_data_ = std::make_unique<uint8_t[]>(pattle_len_);
            file.read((char *)palette_data_.get(), pattle_len_);
        }

        width_ = info_header->bwidth;
        height_ = info_header->bheight;
        color_depth_ = info_header->bBitCount;
        bytes_per_pixel_ = color_depth_ / 8;
        if (bytes_per_pixel_ == 0) {
            bytes_per_pixel_ = 1;
        }
        compress_ = info_header->bCompression;

        image_data_size_ = width_ * height_ * bytes_per_pixel_;
        image_data_ = std::make_unique<uint8_t[]>(image_data_size_);
        file.read((char *)image_data_.get(), image_data_size_);
        
        reserval_image();

        file.close();
        return true;
    }

    void show_info(void)
    {
        std::cout<<"width: "<<width_<<std::endl;
        std::cout<<"height: "<<height_<<std::endl;
        std::cout<<"color_depth: "<<color_depth_<<std::endl;
        std::cout<<"bytes_per_pixel: "<<bytes_per_pixel_<<std::endl;
        std::cout<<"image_data_size_: "<<image_data_size_<<std::endl;
        std::cout<<"compress: "<<compress_<<std::endl;
        std::cout<<"pattle_len_: "<<pattle_len_<<std::endl;
    }

    bool save_to_bmp(const std::string filename, bmp_compress_state compress = bmp_compress_state::IMAGE_NOCOMPRESS)
    {
        uint8_t my_compress = get_compress_value(compress);
        std::ofstream file(filename, std::ios::binary | std::ios::trunc);
        
        if (!file.is_open()) {
            std::cout << "File open failed!" << std::endl;
            return false; 
        }

        if (image_data_ == nullptr) {
            std::cout << "image_data_ is null!" << std::endl;
            return false; 
        }

        //添加bmp文件头
        bmp_file_header file_header;
        bmp_info_header info_header;
        file_header.bfType = BMP_FILE_TYPE;
        file_header.reserved0 = 0;
        file_header.reserved1 = 0;
        file_header.offsets = BMP_HEAD_LEN;

        //添加bmp信息头
        info_header.bsize = BMP_INFOHEADER_LEN;
        info_header.bwidth = width_;
        info_header.bheight = height_;
        info_header.bPlanes = 1;
        info_header.bBitCount = color_depth_;
        info_header.bCompression = my_compress;  //压缩方式
        info_header.bSizeImage = image_data_size_;
        info_header.bXPelsPerMeter = 0;
        info_header.bYPelsPerMeter = 0;
        info_header.bClrUsed = 0;
        info_header.bClrImportant = 0;

        //图像单色, 16色, 256色, 位图数据是调色板的索引值, 需要添加调色板(BytePerPix为1)
        //位图是16位, 24位和32位色, 不存在调色板, 图像的颜色直接在位图数据中给出
        if (bytes_per_pixel_ == 1) {
            file_header.offsets += pattle_len_;
        }
        //位图文件长度(包含图像数据信息，文件头和位图数据的总长度)
        file_header.bfSize = image_data_size_ + file_header.offsets;

        std::unique_ptr<uint8_t[]> p_header = std::make_unique<uint8_t[]>(BMP_HEAD_LEN);
        memcpy(p_header.get(), (char *)&file_header, BMP_FILEHEADER_LEN);
        memcpy(p_header.get() + BMP_FILEHEADER_LEN, (char *)&info_header, BMP_INFOHEADER_LEN);
        file.write((char *)p_header.get(), BMP_HEAD_LEN);
        
        //调位板定义(每个色彩4字节)
        //对于单色, 16色和256色
        if ( bytes_per_pixel_ == 1 ) {
            file.write((char *)palette_data_.get(), pattle_len_);
        }
    
        if ( my_compress == 0 ) {
            //不压缩,写入实际数据
            save_image_nocompress(file);
        } else if ( my_compress == 1 ) {
            //对于256色图像,采用RLE4压缩

        } else {

        }

        file.close();
        return true;
    }

private:
    // 获取压缩方式
    uint8_t get_compress_value(bmp_compress_state compress)
    {
        uint8_t my_compress = 0;

        if (compress == bmp_compress_state::IMAGE_COMPRESS) {
            switch (color_depth_) {
               case 4:
                    my_compress = 2;
                    break;
               case 8:
                    my_compress = 1;
                    break;
               default:
                    std::cout << "not supoort compress, use default!" << std::endl;
                    break;
            }
        }
        return my_compress;   
    }

    void save_image_nocompress(std::ofstream &file)
    {
        uint32_t width_bytes = width_ * bytes_per_pixel_;
        std::unique_ptr<uint8_t[]> cache_buffer = std::make_unique<uint8_t[]>(image_data_size_);

        for(int i = 0; i < height_; i++)
        {
            uint8_t *p_cache = cache_buffer.get() + image_data_size_ - width_bytes * (i + 1);
            uint8_t *p_image = image_data_.get() + width_bytes * i;
            std::memcpy(p_cache, p_image, width_bytes);   
        }

        file.write((char *)cache_buffer.get(), image_data_size_);
    }

    // 反转图像内各行数据，获取真实数据
    void reserval_image(void) 
    {
        uint32_t width_bytes = width_ * bytes_per_pixel_;
        std::unique_ptr<uint8_t[]> cache_buffer = std::make_unique<uint8_t[]>(image_data_size_);

        /*
            反转图像内数据所在行, 原始数据和bmp中的按行顺序相反
        */ 
        std::memcpy(cache_buffer.get(), image_data_.get(), image_data_size_);
        for(int i = 0; i < height_; i++)
        {
            uint8_t *p_cache = cache_buffer.get() + image_data_size_ - width_bytes * (i + 1);
            uint8_t *p_image = image_data_.get() + width_bytes * i;
            std::memcpy(p_image, p_cache, width_bytes);
        }

        std::cout << "reserval image" << std::endl;
    }

private:
    // 图像数据
    std::unique_ptr<uint8_t[]> image_data_{nullptr};

    std::unique_ptr<uint8_t[]> palette_data_{nullptr};

    uint32_t pattle_len_{0};

    // 图形宽度
    uint32_t width_;

    // 图形高度         
    uint32_t height_;

    //每个像素点占的Byte数
    uint16_t bytes_per_pixel_;

    // 数据总数
    uint64_t image_data_size_;

    //图像的色深    
    uint16_t color_depth_;

    // 压缩方式
    uint32_t compress_;
};
}