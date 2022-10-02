#include "res.h"

BmpReader::BmpReader()
{

};

void BmpReader::LoadFile(std::string filepath)
{
    fImage.open(filepath, std::ifstream::binary);
    filename = filepath;
};

bool BmpReader::GetHeader()
{
    //set '\0' to last char in HFiled
    bmpH.HField[2] = '\0';
    //Bitmap file header
    fImage.read(bmpH.HField, sizeof(char) * 2);
    fImage.read(reinterpret_cast<char *>(&bmpH.size), sizeof(int));
    fImage.read(reinterpret_cast<char *>(&bmpH.reserved1), sizeof(unsigned short));
    fImage.read(reinterpret_cast<char *>(&bmpH.reserved2), sizeof(unsigned short));
    fImage.read(reinterpret_cast<char *>(&bmpH.dataOffset), sizeof(int));

    //DIB header
    fImage.read(reinterpret_cast<char *>(&dibH.dibHeaderSize), sizeof(int));
    switch(dibH.dibHeaderSize)
    {
        case BITMAPCOREHEADER:
            dibH.type = "BITMAPCOREHEADER/OS21XBITMAPHEADER";
            break;
        case BITMAPINFOHEADER:
            dibH.type = "BITMAPINFOHEADER";
            break;
        case BITMAPV2INFOHEADER:
            dibH.type = "BITMAPV2INFOHEADER";
            break;
        case BITMAPV3INFOHEADER:
            dibH.type = "BITMAPV3INFOHEADER";
            break;
        case OS22XBITMAPHEADER:
            dibH.type = "OS22XBITMAPHEADER";
            break;
        case BITMAPV4HEADER:
            dibH.type = "BITMAPV4HEADER";
            break;
        case BITMAPV5HEADER:
            dibH.type = "BITMAPV5HEADER";
            break;
        default:
            dibH.type = "UNKNOWN!";
            break;
    }
    if(dibH.dibHeaderSize == BITMAPINFOHEADER)
    {
        fImage.read(reinterpret_cast<char *>(&dibH.width), sizeof(int));
        fImage.read(reinterpret_cast<char *>(&dibH.height), sizeof(int));
        fImage.read(reinterpret_cast<char *>(&dibH.colorPlanes), sizeof(unsigned short));
        fImage.read(reinterpret_cast<char *>(&dibH.bitsPerPixel), sizeof(unsigned short));
        fImage.read(reinterpret_cast<char *>(&dibH.compressionMethod), sizeof(int));
        fImage.read(reinterpret_cast<char *>(&dibH.imageSize), sizeof(int));
        fImage.read(reinterpret_cast<char *>(&dibH.hRes), sizeof(signed int));
        fImage.read(reinterpret_cast<char *>(&dibH.vRes), sizeof(signed int));
        fImage.read(reinterpret_cast<char *>(&dibH.colorsInPalette), sizeof(int));
        fImage.read(reinterpret_cast<char *>(&dibH.importantColors), sizeof(int));
        fImage.seekg(bmpH.dataOffset);
        int pixDataArrSize = ((dibH.bitsPerPixel*dibH.width)/32)*4*dibH.height;
        pixelData = new uint8_t[pixDataArrSize];
        fImage.read(reinterpret_cast<char *>(pixelData), sizeof(uint8_t) * pixDataArrSize);

        /*
        uint8_t *blue = pixelData, *green = blue + 1, *red = green + 1;
        for(int x = 0, y = 0, c = 0; y < dibH.height; x++, blue+=3, green+=3, red+=3)
        {
            if(c+2 >= dibH.width*3*dibH.height)
                break;
            if(x == dibH.width)
            {
                x = 0;
                y++;
            }
            *blue = ~(*blue);
            *red = ~(*red);
            *green = ~(*green);
        }
        */
    }
    else
    {
        fImage.close();
        return false;
    }

    fImage.close();
    return true;
};

void BmpReader::PrintImageInfo()
{
    printf("Image: \"%s\"\n", filename.c_str());
    printf("\tBitmap file header info:\n");
    printf("\t\t---Header field:\t%s\n", bmpH.HField);
    printf("\t\t---Total size in bytes:\t%i\n", bmpH.size);
    printf("\t\t---Data offset:\t\t%i\n", bmpH.dataOffset);
    if(dibH.dibHeaderSize == BITMAPINFOHEADER)
    {
        printf("\tDIB header info:\n");
        printf("\t\t---Header size:\t\t%i\n", dibH.dibHeaderSize);
        printf("\t\t---Header type:\t\t%s\n", dibH.type.c_str());
        printf("\t\t---Width:\t\t%i\n", dibH.width);
        printf("\t\t---Height:\t\t%i\n", dibH.height);
        printf("\t\t---Color planes:\t%i\n", dibH.colorPlanes);
        printf("\t\t---Bits per pixel:\t%i\n", dibH.bitsPerPixel);
        printf("\t\t---Compression method:\t%i\n", dibH.compressionMethod);
        printf("\t\t---Raw bitmap size:\t%i\n", dibH.imageSize);
        printf("\t\t---Res:\t\t\t%i x %i\n", dibH.hRes, dibH.vRes);
        printf("\t\t---Colors in palette:\t%i\n", dibH.colorsInPalette);
        printf("\t\t---Important colors:\t%i\n", dibH.importantColors);
    }
    else
        printf("Unsupported DIB format! (%i)\n", dibH.dibHeaderSize);
};

void BmpReader::SaveOutput()
{
    std::ofstream of;
    of.open("out.bmp", std::ofstream::binary);
    of.write(bmpH.HField, sizeof(char) * 2);
    of.write(reinterpret_cast<char *>(&bmpH.size), sizeof(int));
    of.write(reinterpret_cast<char *>(&bmpH.reserved1), sizeof(unsigned short));
    of.write(reinterpret_cast<char *>(&bmpH.reserved2), sizeof(unsigned short));
    of.write(reinterpret_cast<char *>(&bmpH.dataOffset), sizeof(int));
    of.write(reinterpret_cast<char *>(&dibH.dibHeaderSize), sizeof(int));
    of.write(reinterpret_cast<char *>(&dibH.width), sizeof(int));
    of.write(reinterpret_cast<char *>(&dibH.height), sizeof(int));
    of.write(reinterpret_cast<char *>(&dibH.colorPlanes), sizeof(unsigned short));
    of.write(reinterpret_cast<char *>(&dibH.bitsPerPixel), sizeof(unsigned short));
    of.write(reinterpret_cast<char *>(&dibH.compressionMethod), sizeof(int));
    of.write(reinterpret_cast<char *>(&dibH.imageSize), sizeof(int));
    of.write(reinterpret_cast<char *>(&dibH.hRes), sizeof(signed int));
    of.write(reinterpret_cast<char *>(&dibH.vRes), sizeof(signed int));
    of.write(reinterpret_cast<char *>(&dibH.colorsInPalette), sizeof(int));
    of.write(reinterpret_cast<char *>(&dibH.importantColors), sizeof(int));

    int pixDataArrSize = ((dibH.bitsPerPixel*dibH.width)/32)*4*dibH.height;
    
    of.write(reinterpret_cast<char *>(pixelData), sizeof(uint8_t) * pixDataArrSize);
    

    of.close();
};

void BmpReader::MakeRes()
{
    std::ofstream fHeader("bmp_data.h");
    std::string strHeader = "#ifndef BMP_DATA_H_\n#define BMP_DATA_H_\n\n#include <cstdint>\n#include <stdio.h>\n\nclass BmpData\n{\n\tpublic:\n\t\tBmpData();\n\t\tuint8_t* GetData();\n};\n\n#endif";
    fHeader << strHeader;
    fHeader.close();

    uint8_t *blue, *green, *red;
    std::ofstream fData("bmp_data.cpp");
    std::string strData = "#include \"bmp_data.h\"\n\nBmpData::BmpData()\n{\n\n};\n\nuint8_t* BmpData::GetData()\n{\n\tuint8_t* data = new uint8_t[";
    strData += std::to_string(dibH.width*dibH.height*3);

    strData += "]{";

    blue = new uint8_t[dibH.width*dibH.height];
    green = new uint8_t[dibH.width*dibH.height];
    red = new uint8_t[dibH.width*dibH.height];

    uint8_t* ptrData = pixelData;
    for(int x = 0, y = 0, c = 0; y < dibH.height; x++, c++)
    {
        if(x == dibH.width)
        {
            x = 0;
            y++;
        }
        blue[c] = *ptrData;
        ptrData++;
        green[c] = *ptrData;
        ptrData++;
        red[c] = *ptrData;
        ptrData++;
    }

    for(int i = 0; i < dibH.width*dibH.height; i++)
        strData += std::to_string(red[i]) + ", ";
    for(int i = 0; i < dibH.width*dibH.height; i++)
        strData += std::to_string(green[i]) + ", ";
    for(int i = 0; i < dibH.width*dibH.height; i++)
        strData += std::to_string(blue[i]) + ", ";

    strData.resize(strData.length() - 2);
    strData += "};\n\n\treturn data;\n};";

    fData << strData;
    fData.close();
};