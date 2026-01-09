#ifndef ASCII_IMAGE_H 
#define ASCII_IMAGE_H

#include "types.h" 
hdRawImage* loadRawImage(char* path); 
void writeRawImage(hdRawImage* img, char* path);
hdRawImage* readRawImage(char* path);
uint64_t pixel_hash(const void* item, u64 seed0, u64 seed1);
int pixel_compare(const void* a, const void* b, void *udata);
#endif
