#ifndef TYPES_H_LONGERFILENAMEJUSTINCASECAUSETYPESISKINDAGENERICANDSTUFF
#define TYPES_H_LONGERFILENAMEJUSTINCASECAUSETYPESISKINDAGENERICANDSTUFF

#include <stdint.h> 

typedef uint32_t u32; 
typedef uint16_t u16; 
typedef uint8_t u8;

typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef struct pixel {
	u8 r, g, b; 
	char c; 
	//please no padding mr c
} __attribute__((packed)) hdPixel; //NO PADDING MR GCC THANK YOU 

typedef struct rawImage {
	u16 size_x; 
	u16 size_y; 
	hdPixel* map; //hd stands for header btw
} hdRawImage;

typedef struct compressedPixel {
	u16 count; 
	u16 pos; 
} hdCompressedPixel; 

typedef struct thepalwhoate {
	u32 size; 
	hdPixel* arr;
} hdPixelPalette;

typedef struct compressedMap {
	u16 size_x; 
	u16 size_y; 
	hdCompressedPixel* map;
	hdPixelPalette* palette;
} hdCompressedMap;


#endif
