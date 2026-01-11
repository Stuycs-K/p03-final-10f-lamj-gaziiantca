#ifndef ASCII_IMAGE_H 
#define ASCII_IMAGE_H

#include "types.h" 
hdRawImage* loadRawImage(const char* path); 
void writeRawImage(const hdRawImage* img, const char* path);
hdRawImage* readRawImage(const char* path);
uint64_t pixel_hash(const void* item, u64 seed0, u64 seed1);
void nukeRawImage(hdRawImage* img);
int pixel_compare(const void* a, const void* b, void *udata);
hdCompressedImage* compressRawImage(const hdRawImage* img, hdPixelPalette* palette);
hdRawImage* uncompressImage(const hdCompressedImage* img);
void writeCompressedImage(const hdCompressedImage* img, const char* path);
hdCompressedImage* readCompressedImage(const char* path, hdPixelPalette* palette);
#endif
