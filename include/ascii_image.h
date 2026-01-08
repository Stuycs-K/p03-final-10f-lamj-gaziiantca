#ifndef ASCII_IMAGE_H 
#define ASCII_IMAGE_H

#include "types.h" 
hdRawImage* loadRawImage(char* path); 
void writeRawImage(hdRawImage* img, char* path);
hdRawImage* readRawImage(char* path);
#endif
