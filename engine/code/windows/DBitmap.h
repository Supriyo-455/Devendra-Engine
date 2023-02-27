#pragma once

#include "defines.h"
#include <Windows.h>
#include <stdio.h>
#include "Devendra_Math.h"

struct image
{
    uint32_t Width;
    uint32_t Height;
    uint32_t *Pixels;
};

inline uint32 RoundReal32ToUint32(real32 f)
{
    uint32 Result = (uint32)(f + 0.5);
    return Result;
}

inline uint32 BGRAPack4x8(vec4 Unpacked)
{
    uint32 Result = ((RoundReal32ToUint32(Unpacked.w) << 24) |
                     (RoundReal32ToUint32(Unpacked.x) << 16) |
                     (RoundReal32ToUint32(Unpacked.y) << 8) |
                     (RoundReal32ToUint32(Unpacked.z) << 0));

    return Result;
}

inline uint32 RGBAPack4x8(vec4 Unpacked)
{
    uint32 Result = ((RoundReal32ToUint32(Unpacked.x) << 24) |
                     (RoundReal32ToUint32(Unpacked.y) << 16) |
                     (RoundReal32ToUint32(Unpacked.z) << 8) |
                     (RoundReal32ToUint32(Unpacked.w) << 0));

    return Result;
}

inline vec4 RGBAUnpack4x8(uint32 Packed)
{
    vec4 Result = {};
    Result.x = (real32)((Packed >> 0) & 0xFF);
    Result.y = (real32)((Packed >> 8) & 0xFF);
    Result.z = (real32)((Packed >> 16) & 0xFF);
    Result.w = (real32)((Packed >> 24) & 0xFF);
    return Result;
}

inline real32 LinearTosRGB(real32 L)
{
    if (L < 0.0001f)
        L = 0.0f;
    if (L > 1.0f)
        L = 1.0f;

    real32 S = L * 12.92f;
    ;
    if (L > 0.0031308f)
    {
        S = (1.055f * powf(L, (1.0f / 2.4f))) - 0.055f;
    }
    return S;
}

uint32_t GetTotalPixelSize(image Image)
{
    return sizeof(uint32_t) * Image.Height * Image.Width;
}

image CreateImage(uint32_t Width, uint32_t Height)
{
    image Image = {};
    Image.Width = Width;
    Image.Height = Height;
    uint32_t OutputPixelSize = GetTotalPixelSize(Image);
    Image.Pixels = (uint32_t *)malloc(OutputPixelSize);
    return Image;
}

uint32 GetOutputColor(vec3 Color, uint32 SamplesPerPixel, bool32 useGammaCorrection = false)
{
    real32 Scale = 1.0f / SamplesPerPixel;

    real32 r = Scale * Color.x;
    real32 g = Scale * Color.y;
    real32 b = Scale * Color.z;
    if (useGammaCorrection)
    {
        r = LinearTosRGB(Color.x);
        g = LinearTosRGB(Color.y);
        b = LinearTosRGB(Color.z);
    }

    vec4 BMPColor = vec(255.0f * Clamp(r, 0.0f, 0.999f),
                        255.0f * Clamp(g, 0.0f, 0.999f),
                        255.0f * Clamp(b, 0.0f, 0.999f),
                        255.0f);
    uint32 BMPValue = BGRAPack4x8(BMPColor);
    return BMPValue;
}

void WriteImage(image Image, char *OutputFileName)
{
    // Bitmap structures to be written to file
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

    uint32_t OutputPixelSize = GetTotalPixelSize(Image);

    // Fill BITMAPFILEHEADER structure
    memcpy((char *)&bfh.bfType, "BM", 2);
    bfh.bfSize = sizeof(bfh) + sizeof(bih) + OutputPixelSize;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfOffBits = sizeof(bfh) + sizeof(bih);

    // Fill BITMAPINFOHEADER structure
    bih.biSize = sizeof(bih);
    bih.biWidth = Image.Width;
    bih.biHeight = Image.Height;
    bih.biPlanes = 1;
    bih.biBitCount = 32;
    bih.biCompression = BI_RGB; // uncompressed 24-bit RGB
    bih.biSizeImage = 0;        // can be zero for BI_RGB bitmaps
    bih.biXPelsPerMeter = 4000;
    bih.biYPelsPerMeter = 4000;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    FILE *OutFile = fopen(OutputFileName, "wb");
    if (OutFile)
    {
        fwrite(&bfh, 1, sizeof(bfh), OutFile);
        fwrite(&bih, 1, sizeof(bih), OutFile);
        fwrite(Image.Pixels, OutputPixelSize, 1, OutFile);
        fclose(OutFile);
    }
}