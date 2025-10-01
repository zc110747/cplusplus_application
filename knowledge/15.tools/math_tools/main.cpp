//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by Person Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <cassert>
#include <cstring>
#include "math_tools.hpp"

using MATH_TOOLS::math_tools;

void math_crc_schedular(void) 
{
    uint8_t buffer[] = "this is a test string";

    printf("calc_crc8: 0x%02X\n", math_tools::get_instance()->calc_crc8(buffer, strlen((char *)buffer)));
    printf("calc_crc16: 0x%04X\n", math_tools::get_instance()->calc_crc16(buffer, strlen((char *)buffer)));
    printf("calc_crc32: 0x%04X\n", math_tools::get_instance()->calc_crc32(buffer, strlen((char *)buffer)));

    printf("calc_crc8: 0x%02X\n", math_tools::get_instance()->crc8_search_table(buffer, strlen((char *)buffer)));

    assert(math_tools::get_instance()->calc_crc8(buffer, strlen((char *)buffer)) 
        == math_tools::get_instance()->crc8_search_table(buffer, strlen((char *)buffer)));

    // 比较直接计算法和查表法的结果，确保两者一致
    for (size_t i = 0; i < 20; i++) {
        uint8_t calc_val, calc_list_val;
        uint8_t buf_in[4];
         
        math_tools::get_instance()->get_random_bytes(buf_in, sizeof(buf_in));

        calc_val = math_tools::get_instance()->calc_crc8(buf_in, sizeof(buf_in));
        calc_list_val = math_tools::get_instance()->crc8_search_table(buf_in, sizeof(buf_in));
        assert(calc_val == calc_list_val);
    }
}

void math_md5_schedular(void)
{
    uint8_t buffer[] = "this is a test string";
    // md5 32位计算
    uint8_t md5_val[EVP_MAX_MD_SIZE];
    uint16_t md5_len = math_tools::get_instance()->calc_md5(buffer, strlen((char *)buffer), md5_val);
    
    printf("calc md5-32:");
    for(unsigned int i = 0; i < md5_len; i++) {
        printf("%02x", md5_val[i]);     //486eb65274adb86441072afa1e2289f3
    }
    printf("\n");

    // md5 16位，为提取9到25位
    printf("calc md5-16:");
    for(unsigned int i = 4; i < 12; i++) {
        printf("%02x", md5_val[i]);     //74adb86441072afa
    }
    printf("\n");    
}

#define AES_KEY_LENGTH 32
#define AES_BLOCK_SIZE 16

void math_aes_schedular(void)
{
    MATH_TOOLS::math_tools *tools = MATH_TOOLS::math_tools::get_instance();

    const unsigned char key[AES_KEY_LENGTH+1] = "01234567890123456789012345678901";
    unsigned char iv[AES_BLOCK_SIZE];
    
    tools->get_random_bytes(iv, AES_BLOCK_SIZE);

    printf("calc aes iv:");
    for(unsigned int i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%02x", iv[i]);
    }
    printf("\n");

    const unsigned char *plaintext = (const unsigned char *)"Hello, AES!";
    int plaintext_len = strlen((const char *)plaintext);

    unsigned char ciphertext[1024];
    unsigned char decryptedtext[1024];

    int ciphertext_len = math_tools::get_instance()->aes_encrypt(plaintext, plaintext_len, key, iv, ciphertext);
    if (ciphertext_len > 0) {
        printf("calc aes:");
        for(unsigned int i = 0; i < ciphertext_len; i++) {
            printf("%02x", ciphertext[i]);     
        }
        printf("\n");
        
        int decryptedtext_len = math_tools::get_instance()->aes_decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);
        if (decryptedtext_len > 0) {
            decryptedtext[decryptedtext_len] = '\0';
            std::cout << "Decrypted text: " << decryptedtext << std::endl;
        }
    }  
}

int main(int argc, char **argv) 
{
    math_crc_schedular();

    math_md5_schedular();

    math_aes_schedular();
    
    return 0;
}
