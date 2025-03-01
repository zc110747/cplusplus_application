/*

*/

#include <iostream>
#include "image.hpp"

using namespace std;

#define BMP_IMAGE "bmp/bmp_24.bmp"

int main(int argc, char *argv[])
{
    bmp_img::bmp_decoder decoder;
    
    if(!decoder.decode_bmp(BMP_IMAGE))
    {
        cout << "decode bmp failed" << endl;
        return -1; 
    } 
    decoder.show_info();
    decoder.save_to_bmp("test.bmp");

    return 1;
}