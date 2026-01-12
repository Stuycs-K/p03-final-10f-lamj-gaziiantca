#ifndef ASCII_IMAGE_H 
#define ASCII_IMAGE_H

#include "types.h" 
hdRawImage* loadRawImage(const char* path); //Takes in a path to a file outputed by ascii-image-converter and parses it into an hdRawImage 
void writeRawImage(const hdRawImage* img, const char* path); //Writes an hdRawImage to a file
hdRawImage* readRawImage(const char* path); //Reads an hdRawImage that was written with the writeRawImage() function
void nukeRawImage(hdRawImage* img); //free()s everything related to the raw map

uint64_t pixel_hash(const void* item, u64 seed0, u64 seed1); //hashes a pixel
int pixel_compare(const void* a, const void* b, void *udata); //compares two hdHashEntrys' pixels
															  
hdCompressedImage* compressRawImage(const hdRawImage* img, hdPixelPalette* palette); //Takes in an hdRawImage and compresses it. Also takes in an optional palette. Useful if you know that two images share their colors (example use case: playing bad apple)
hdRawImage* uncompressImage(const hdCompressedImage* img); //Takes in an hdCompressedImage* and unrolls it back into a raw image

void writeCompressedImage(const hdCompressedImage* img, const char* path); //Writes a compressed image to a file
hdCompressedImage* readCompressedImage(const char* path, hdPixelPalette* palette); //Reads a compressed image from a file that was written to using writeCompressedImage(). Also takes in an optional palette.

void nukePalette(hdPixelPalette* palette); //free()s everything related to the palette
void cleanupCompressedImage(hdCompressedImage* img); //free()s most things in compressed image, notably doesn't nuke the palette. 
void nukeCompressedImage(hdCompressedImage* img); //free()s everything related to a compressedImage by running nukePalette() and cleanupCompressedImage()
#endif
