//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by Person Inc.  
//  All Rights Reserved
//
//  Name:
//      math_tools.hpp
//
//  Purpose:
//
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#ifndef _MATH_TOOLS_HPP_
#define _MATH_TOOLS_HPP_

#include <stdint.h>
#include <stdlib.h>
#include <openssl/evp.h> // 包含 OpenSSL 通用消息摘要头文件

#include <random>
#include <iostream>

// 定义 CRC8模式的多项式和初始值
#define CRC8_POLYNOMIAL        0x05
#define CRC8_INIT              0xFF

// 定义 CRC16模式的多项式和初始值
#define CRC16_POLYNOMIAL        0x8005
#define CRC16_INIT              0xFFFF

// 定义CRC32模式的多项式和初始值
#define CRC32_POLYNOMIAL        0x04C11DB7
#define CRC32_INIT              0xFFFFFFFF

namespace MATH_TOOLS
{
    class math_tools final {
        public:
            /// @brief constructor
            math_tools(){}

            /// @brief destructor
            ~math_tools() {}

            /// @brief copy constructor and operator= deleted 
            math_tools(const math_tools&) = delete;
            math_tools& operator=(const math_tools&) = delete;

            /// @brief get instance
            /// - get static instance of math_tools
            /// @return math_tools*
            static math_tools *get_instance() {
                static math_tools instance;
                return &instance;
            }

            /// @brief calc_crc8
            /// - calc crc8 value for input data
            /// @param ptr input data
            /// @param len input data length
            /// @return crc8 check value
            uint8_t calc_crc8(const uint8_t *ptr, uint32_t len)
            {
                uint8_t i;
                uint8_t crc = CRC8_INIT;

                // 遍历输入数据
                while (len--) {
                    crc ^= *ptr++;
                    // 处理每个字节的 8 位
                    for (i = 0; i < 8; ++i) {
                        if (crc & 0x80) {
                            crc = (crc << 1) ^ CRC8_POLYNOMIAL;
                        } else {
                            crc = (crc << 1);
                        }
                    }
                }
                return crc;
            }

            /// @brief calc_crc16
            /// - calc crc16 value for input data
            /// @param ptr input data
            /// @param len input data length
            /// @return crc16 check value
            uint16_t calc_crc16(const uint8_t *ptr, uint32_t len) {
                uint8_t i;
                uint16_t crc = CRC16_INIT;

                // 遍历输入数据
                while (len--) {
                    crc ^= (uint16_t)*ptr++ << 8;
                    // 处理每个字节的 8 位
                    for (i = 0; i < 8; ++i) {
                        if (crc & 0x8000) {
                            crc = (crc << 1) ^ CRC16_POLYNOMIAL;
                        } else {
                            crc = (crc << 1);
                        }
                    }
                }
                return crc;
            }
            
            /// @brief calc_crc32
            /// - calc crc32 value for input data
            /// @param ptr input data
            /// @param len input data length
            /// @return crc32 check value
            static uint32_t calc_crc32(const uint8_t *ptr, uint32_t len) {
                uint8_t i;
                uint32_t crc = CRC32_INIT;

                // 遍历输入数据
                while (len--) {
                    crc ^= (uint16_t)*ptr++ << 24;
                    // 处理每个字节的 8 位
                    for (i = 0; i < 8; ++i) {
                        if (crc & 0x80000000) {
                            crc = (crc << 1) ^ CRC32_POLYNOMIAL;
                        } else {
                            crc = (crc << 1);
                        }
                    }
                }
                return crc;
            }

            /// @brief generate_crc8_table
            /// - generate crc8 table for search
            /// @param polynomial crc8 polynomial
            void generate_crc8_table(uint8_t polynomial) {
                for (int i = 0; i < 256; i++) {
                    uint8_t crc = i;
                    for (int j = 0; j < 8; j++) {
                        if (crc & 0x80) {
                            crc = (crc << 1) ^ polynomial;
                        } else {
                            crc <<= 1;
                        }
                    }
                    crc8_table_[i] = crc;
                }
            }

            /// @brief crc8_search_table
            /// - search crc8 value in table for input data
            /// @param ptr input data
            /// @param len input data length
            /// @return crc8 check value
            uint8_t crc8_search_table(const uint8_t *ptr, uint32_t len) {
                uint8_t crc = CRC8_INIT;

                if (!is_table_generated_) {
                    generate_crc8_table(CRC8_POLYNOMIAL);
                    is_table_generated_ = true;
                }

                for (size_t i = 0; i < len; i++) {
                    crc = crc8_table_[crc ^ ptr[i]];
                }
                return crc;
            }

            /// @brief md5_calc
             // MD5 计算函数实现
            uint16_t calc_md5(uint8_t *ptr, uint32_t len, uint8_t *result) {
                EVP_MD_CTX *mdctx;
                const EVP_MD *md;
                unsigned int md_len;

                // 初始化上下文
                mdctx = EVP_MD_CTX_new();
                md = EVP_md5();

                // 初始化摘要计算
                EVP_DigestInit_ex(mdctx, md, NULL);

                // 更新摘要
                EVP_DigestUpdate(mdctx, ptr, len);

                // 完成摘要计算
                EVP_DigestFinal_ex(mdctx, result, &md_len);

                // 释放上下文
                EVP_MD_CTX_free(mdctx);

                return md_len;
            }

                        /// @brief aes_encrypt
            /// - AES-256-CBC 加密函数
            /// @param plaintext 明文数据
            /// @param plaintext_len 明文数据长度
            /// @param key 256 位（32 字节）密钥
            /// @param iv 初始化向量（16 字节）
            /// @param ciphertext 存储密文的缓冲区
            /// @return 加密后的密文长度
            int aes_encrypt(const unsigned char *plaintext, int plaintext_len,
                            const unsigned char *key, const unsigned char *iv,
                            unsigned char *ciphertext) {
                EVP_CIPHER_CTX *ctx;
                int len;
                int ciphertext_len;

                // 创建并初始化上下文
                if (!(ctx = EVP_CIPHER_CTX_new())) {
                    return -1;
                }

                // 初始化加密操作
                if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
                    EVP_CIPHER_CTX_free(ctx);
                    return -1;
                }

                // 提供明文并进行加密
                if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
                    EVP_CIPHER_CTX_free(ctx);
                    return -1;
                }
                ciphertext_len = len;

                // 结束加密操作
                if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
                    EVP_CIPHER_CTX_free(ctx);
                    return -1;
                }
                ciphertext_len += len;

                // 释放上下文
                EVP_CIPHER_CTX_free(ctx);

                return ciphertext_len;
            }

            void get_random_bytes(uint8_t *buffer, size_t length) {
                // 创建一个随机数生成器
                std::random_device rd;
                std::mt19937 gen(rd());

                // 创建一个均匀分布，生成 1 到 255 之间的随机数
                std::uniform_int_distribution<> dis(1, 255);

                for (int n = 0; n < length; ++n) {
                    buffer[n] = dis(gen);
                }
            }

            /// @brief aes_decrypt
            /// - AES-256-CBC 解密函数
            /// @param ciphertext 密文数据
            /// @param ciphertext_len 密文数据长度
            /// @param key 256 位（32 字节）密钥
            /// @param iv 初始化向量（16 字节）
            /// @param plaintext 存储明文的缓冲区
            /// @return 解密后的明文长度
            int aes_decrypt(const unsigned char *ciphertext, int ciphertext_len,
                            const unsigned char *key, const unsigned char *iv,
                            unsigned char *plaintext) {
                EVP_CIPHER_CTX *ctx;
                int len;
                int plaintext_len;

                // 创建并初始化上下文
                if (!(ctx = EVP_CIPHER_CTX_new())) {
                    return -1;
                }

                // 初始化解密操作
                if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
                    EVP_CIPHER_CTX_free(ctx);
                    return -1;
                }

                // 提供密文并进行解密
                if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
                    EVP_CIPHER_CTX_free(ctx);
                    return -1;
                }
                plaintext_len = len;

                // 结束解密操作
                if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
                    EVP_CIPHER_CTX_free(ctx);
                    return -1;
                }
                plaintext_len += len;

                // 释放上下文
                EVP_CIPHER_CTX_free(ctx);

                return plaintext_len;
            }
            
        private:
            /// @brief crc8_table_
            /// - crc8 table for search
            uint8_t crc8_table_[256];

            /// @brief is_table_generated_
            /// - is crc8 table generated
            bool is_table_generated_{false};
    };
}


#endif
