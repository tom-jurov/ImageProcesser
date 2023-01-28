#ifndef TOMAS_IMAGE_H
#define TOMAS_IMAGE_H
#include <cstdint>

enum class ImageType {
    PNG, JPG, BMP, TGA
};

struct Image
{
    uint8_t* data = nullptr;
    std::size_t size = 0;
    int w;
    int h;
    int channels;

    Image(const char* filename);
    Image(int w, int h, int channels);
    Image(const Image& other);
    ~Image();

    bool read(const char* filename);
    bool write(const char* filename);
    ImageType getFileType(const char* filename);

    Image& grayscale();
    Image& grayscale_lum();

    Image& colorMask(float r, float g, float b);

    Image& encodeMessage(const char* message);
    void decodeMessage(char* buffer, std::size_t& messageLength);

};
#endif