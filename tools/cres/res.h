#ifndef RES_H_
#define RES_H_

#include <fstream>
#include <string>
#include <stdio.h>

enum DIBTypes
{
    BITMAPCOREHEADER = 12,
    OS21XBITMAPHEADER = 12,
    BITMAPINFOHEADER = 40,
    BITMAPV2INFOHEADER = 52,
    BITMAPV3INFOHEADER = 56,
    OS22XBITMAPHEADER = 64,
    BITMAPV4HEADER = 108,
    BITMAPV5HEADER = 124
};


struct BFH
{
    char                HField[3];
    int                 size;
    unsigned short      reserved1 = 0;
    unsigned short      reserved2 = 0;
    int                 dataOffset;
};

struct DIBHeader
{
    int                 dibHeaderSize;
    std::string         type;
    int                 width;
    int                 height;
    unsigned short      colorPlanes;
    unsigned short      bitsPerPixel;
    int                 compressionMethod;
    int                 imageSize;
    signed int          hRes;
    signed int          vRes;
    int                 colorsInPalette;
    int                 importantColors;
};

class BmpReader
{
    public:
        BmpReader();
        void LoadFile(std::string filepath);
        bool GetHeader();
        void PrintImageInfo();
        void SaveOutput();
        void MakeRes();

        std::ifstream           fImage;
        std::string             filename;
        BFH                     bmpH;
        DIBHeader               dibH; 

        uint8_t*                pixelData;
};

#endif