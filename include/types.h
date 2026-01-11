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

typedef struct rawImage { //this is what images are stored with in the screen 
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
	//side note but holy moly computer scientists need to stop using the word "map" this is getting riduculous (this comment no longer makes contextual sense due to a renaming however my point still stands independently of it)
} hdPixelPalette;

typedef struct compressedImage { //this is what images are stored with in the game files
	u16 size_x; 
	u16 size_y; 
	hdCompressedPixel* items;
	u32 capacity;
	u32 count;
	hdPixelPalette* palette;
} hdCompressedImage;

typedef struct sprite {
	hdRawImage* image;
	u16 pos_x;
	u16 pos_y; 
	void* extra; //I don't think this is useful to my purposes but since this is a library the user may want to store extra info
} hdSprite;

typedef struct camera {
	u16 pos_x; 
	u16 pos_y; 
	double theta; //Potential trolling for the future
} hdCamera;

typedef struct hdScreen { 
	//This doesn't store y and x sizes because those may change at runtime and ncurses will tell you that infinitely more accurately than I can ever dream of 
	u32 count;
	u32 capacity; 
	hdRawImage* items;
	
} hdScreen;


#endif
