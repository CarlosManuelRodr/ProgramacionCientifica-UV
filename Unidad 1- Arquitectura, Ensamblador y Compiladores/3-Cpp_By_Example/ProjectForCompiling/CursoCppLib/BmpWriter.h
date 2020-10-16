#ifndef __bmpWriter
#define __bmpWriter

#include <fstream>

// Common binary operations
typedef short unsigned int uint16;
typedef unsigned int uint32;
typedef int int32;

char* ToByte(void* ptr)
{
    return static_cast<char*>(ptr);
}

// Invierte extremidad de los bytes.
uint16 SwitchEndianess16(uint16 data)
{
    uint16 temp;
    uint16 out = 0x0000;
    uint16 pos = 0x00FF;

    temp = data & pos;
    temp <<= 8;
    out |= temp;
    pos <<= 8;

    temp = data & pos;
    temp >>= 8;
    out |= temp;

    return out;
}

uint32 SwitchEndianess32(uint32 data)
{
    uint32 temp;
    uint32 out = 0x00000000;
    uint32 pos = 0x000000FF;

    temp = data & pos;
    temp <<= 24;
    out |= temp;
    pos <<= 8;
    temp = data & pos;
    temp <<= 8;
    out |= temp;
    pos <<= 8;
    temp = data & pos;
    temp >>= 8;
    out |= temp;
    pos <<= 8;
    temp = data & pos;
    temp >>= 24;
    out |= temp;

    return out;
}

// Operaciones de conversión.
void BoolToChar(char& target, bool in)
{
    if (in) target = 0x01;
    else target = 0x00;
}

void CharToBool(bool& target, char in)
{
    if (in == 0x00) target = false;
    else target = true;
}

void UInt32ToChar(char* target, uint32 in)
{
    in = SwitchEndianess32(in);
    uint32 temp = 0x000000FF;
    for (int i = 0; i < sizeof(uint32); i++)
    {
        target[i] = in & temp;
        in >>= 8;
    }
}

void CharToUInt32(uint32& target, char* in)
{
    target = 0x00000000;
    uint32 temp;
    for (int i = 0; i < sizeof(uint32); i++)
    {
        temp = in[i] & 0x000000FF;
        target |= temp;
        if (i != 3) target <<= 8;
    }
}

// BMP Implementation

struct BMPHeader
{
    uint16 identifier;
    uint32 size;
    uint16 appSpecific1;
    uint16 appSpecific2;
    uint32 bitmapData;
};

struct DIBHeader
{
    uint32 headerSize;
    int32 width;
    int32 height;
    uint16 nPlanes;
    uint16 colorDepth;
    uint32 compression;
    uint32 bmpBytes;
    int32 hRes;
    int32 vRes;
    uint32 nColors;
    uint32 nImpColors;
};

struct BMPPixel
{
public:
    char r, g, b;
    BMPPixel()
    {
        r = 0;
        g = 0;
        b = 0;
    }
    BMPPixel(char mR, char mG, char mB)
    {
        r = mR;
        g = mG;
        b = mB;
    }
};

typedef struct BMPPixel Color;

class BMPWriter
{
    BMPHeader* myBmpHdr;
    DIBHeader* myDibHdr;
    std::ofstream file;
    unsigned int imageWidth;
    unsigned int imageHeight;
    unsigned int paddingBytes;
    int dataSize;
    unsigned int indexHeight;

public:
    BMPWriter(const char* filepath, unsigned int width, unsigned int height)
    {
        // Crea encabezado
        indexHeight = 0;
        imageWidth = width;
        imageHeight = height;
        dataSize = width * height;
        myBmpHdr = new BMPHeader;    // Para ver lo que sucede en la memoria (depuracion).
        myDibHdr = new DIBHeader;

        myBmpHdr->identifier = SwitchEndianess16(0x424d);    // Windows
        myBmpHdr->appSpecific1 = 0x0000;
        myBmpHdr->appSpecific2 = 0x0000;

        myDibHdr->width = width;
        myDibHdr->height = height;
        myDibHdr->nPlanes = SwitchEndianess16(0x0100);
        myDibHdr->colorDepth = SwitchEndianess16(0x1800);
        myDibHdr->compression = 0x00000000;
        myDibHdr->bmpBytes = SwitchEndianess32(0x10000000);
        myDibHdr->hRes = SwitchEndianess32(0x130B0000);
        myDibHdr->vRes = SwitchEndianess32(0x130B0000);
        myDibHdr->nColors = 0x00000000;
        myDibHdr->nImpColors = 0x00000000;

        unsigned int bmpSize = 0;
        unsigned int offsetData = 54;
        paddingBytes = imageWidth % 4;
        // Calcula tamaño de archivo
        bmpSize += 14;                           //BMPHeader size.
        bmpSize += 40;                           //DIBHeader size
        bmpSize += 3 * width * height;
        bmpSize += imageHeight * paddingBytes;
        myBmpHdr->size = bmpSize;
        myBmpHdr->bitmapData = offsetData;
        myDibHdr->headerSize = 40;               //DIBHeader size

        // Escribe encabezado
        file.open(filepath, std::ios::out | std::ios::binary);
        file.write(ToByte(&myBmpHdr->identifier), 2);
        file.write(ToByte(&myBmpHdr->size), 4);
        file.write(ToByte(&myBmpHdr->appSpecific1), 2);
        file.write(ToByte(&myBmpHdr->appSpecific2), 2);
        file.write(ToByte(&myBmpHdr->bitmapData), 4);
        file.write(ToByte(&myDibHdr->headerSize), 4);
        file.write(ToByte(&myDibHdr->width), 4);
        file.write(ToByte(&myDibHdr->height), 4);
        file.write(ToByte(&myDibHdr->nPlanes), 2);
        file.write(ToByte(&myDibHdr->colorDepth), 2);
        file.write(ToByte(&myDibHdr->compression), 4);
        file.write(ToByte(&myDibHdr->bmpBytes), 4);
        file.write(ToByte(&myDibHdr->hRes), 4);
        file.write(ToByte(&myDibHdr->vRes), 4);
        file.write(ToByte(&myDibHdr->nColors), 4);
        file.write(ToByte(&myDibHdr->nImpColors), 4);
    }
    ~BMPWriter()
    {
        delete myBmpHdr;
        delete myDibHdr;
    }
    void WriteLine(BMPPixel* data)
    {
        if (indexHeight < imageHeight)
        {
            for (unsigned int i = 0; i < imageWidth; i++)
            {
                file.write(&data[i].b, 1);
                file.write(&data[i].g, 1);
                file.write(&data[i].r, 1);
            }
            if (paddingBytes == 1)
            {
                char padding = 0x00;
                file.write(ToByte(&padding), 1);
            }
            if (paddingBytes == 2)
            {
                short padding = 0x0000;
                file.write(ToByte(&padding), 2);
            }
            if (paddingBytes == 3)
            {
                unsigned int padding = 0x00000000;
                file.write(ToByte(&padding), 3);
            }
        }
        indexHeight++;
    }
    void CloseBMP()
    {
        file.close();
    }
};

#endif