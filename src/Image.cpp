#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>

#define STEG_HEADER_SIZE sizeof(uint32_t) * 8


Image::Image(const char* filename)
{
    if(read(filename))
    {
        std::cout << "Read " << filename << std::endl;
        size = w*h*channels;
    }
    else
        std::cout << "Failed to read " << filename << std::endl;
}
Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels)
{
    size = w*h*channels;
    data = new uint8_t[size];
}
Image::Image(const Image& other) : Image(other.w, other.h, other.channels)
{
    memcpy(data, other.data, size);
}

Image::~Image()
{
    stbi_image_free(data);
}
bool Image::read(const char* filename)
{
    data = stbi_load(filename, &w, &h, &channels, 0);
    return data != nullptr;
}
bool Image::write(const char* filename)
{
    ImageType type = getFileType(filename);
    int success = 0;
    switch(type)
    {
        case ImageType::PNG:
            success = stbi_write_png(filename, w, h, channels, data, w*channels);
            break;
        case ImageType::BMP:
            success = stbi_write_bmp(filename, w, h, channels, data);
            break;
        case ImageType::JPG:
            success = stbi_write_jpg(filename, w, h, channels, data, 100);
            break;
        case ImageType::TGA:
            success = stbi_write_tga(filename, w, h, channels, data);
            break;

    }
    return success;
}
Image& Image::grayscale()
{
    if (channels < 3)
    {
        std::cout << "Already grayscale";
    }
    else
    {
        for (int i=0; i < size; i+=channels)
        {
            int gray = (data[i] + data[i+1] + data[i+2])/3;
            memset(data+i, gray, 3);
        }
    }
    return *this;
}
Image& Image::grayscale_lum()
{
    if (channels < 3)
    {
        std::cout << "Already grayscale";
    }
    else
    {
        for (int i=0; i < size; i+=channels)
        {
            int gray = 0.2126*data[i] + 0.7152*data[i+1] + 0.0722*data[i+2];
            memset(data+i, gray, 3);
        }
    }
    return *this;

}
Image& Image::colorMask(float r, float g, float b)
{
    if (channels < 3)
    {
        std::cout << "Not RGB model" << std::endl;
    }
    else
    {
        for (int i=0; i < size; i+=channels)
        {
            data[i] *= r;
            data[i+1] *= g;
            data[i+2] *= b;
        }
    }
    return *this;

}

Image& Image::encodeMessage(const char* message)
{
    uint32_t len = strlen(message) * 8;

    if(len + STEG_HEADER_SIZE > size){
        std::cout << "This message is too large" << std::endl;
        return *this;
    }
    for(uint8_t i=0; i < STEG_HEADER_SIZE; ++i)
    {
        data[i] &= 0xFE;
        data[i] |= (len >> (STEG_HEADER_SIZE -1 - i)) & 1UL;
    }
    for(uint32_t i=0; i < len; ++i)
    {
        data[i+STEG_HEADER_SIZE] &= 0xFE;
        data[i+STEG_HEADER_SIZE] |= (message[i/8] >> ((len - 1 - i)%8)) & 1;
    }

    return *this;
}
void Image::decodeMessage(char* buffer, std::size_t& messageLength)
{
    uint32_t len = 0;
    for(uint8_t i=0; i < STEG_HEADER_SIZE; ++i)
    {
        len = (len << 1) | (data[i] & 1);
    }
    messageLength = len/8;
    for(uint32_t i=0; i < len; ++i)
    {
        buffer[i/8] = (buffer[i/8] << 1) | (data[i + STEG_HEADER_SIZE] & 1);
    }

}

ImageType Image::getFileType(const char *filename)
{
    const char* ext = strrchr(filename, '.');
    if(ext != nullptr)
    {
        if(strcmp(ext, ".png") == 0)
        {
            return ImageType::PNG;
        }
        else if(strcmp(ext, ".jpg") == 0)
        {
            return ImageType::JPG;
        }
        else if(strcmp(ext, ".bmp") == 0)
        {
            return ImageType::BMP;
        }
        else if(strcmp(ext, ".tga") == 0)
        {
            return ImageType::TGA;
        }
    }
    return ImageType::PNG;
}