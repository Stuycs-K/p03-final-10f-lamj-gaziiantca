#ifndef TYPES_H_LONGERFILENAMEJUSTINCASECAUSETYPESISKINDAGENERICANDSTUFF
#define TYPES_H_LONGERFILENAMEJUSTINCASECAUSETYPESISKINDAGENERICANDSTUFF

#include <stdint.h> 

typedef uint64_t u64; 
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
	hdPixel* arr; //hd stands for header btw
} hdRawImage;

typedef struct compressedPixel {
	u16 count; 
	u32 pos; // :(
} __attribute__((packed))  hdCompressedPixel; 

typedef struct hashmapentrybroidkatp {
	hdPixel* pixel; 
	u32 pos;
} hdHashEntry;

typedef struct thepalwhoate {
	u32 capacity; 
	u32 count;
	hdPixel* items;
	struct hashmap *hashmap; 
	//the map will contain the pixels pointing to the indices of said pixels in arr
	//side note but holy moly computer scientists need to stop using the word "map" this is getting riduculous
} hdPixelPalette;

typedef struct compressedImage {
	u16 size_x; 
	u16 size_y; 
	hdCompressedPixel* items;
	u32 capacity;
	u32 count;
	hdPixelPalette* palette;
} hdCompressedImage;


#endif
