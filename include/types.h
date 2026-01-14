#ifndef TYPES_H_LONGERFILENAMEJUSTINCASECAUSETYPESISKINDAGENERICANDSTUFF
#define TYPES_H_LONGERFILENAMEJUSTINCASECAUSETYPESISKINDAGENERICANDSTUFF

#include <stdint.h> 
#include "vector2.h"
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
	hdRawImage* image; //can technically re-use sprites
	i16 pos_x;
	i16 pos_y;
	void* extra; //I don't think this is useful to my purposes but since this is a library and the user may want to store extra info this could be useful
} hdSprite;

typedef struct camera {
	i16 pos_x; 
	i16 pos_y; 
	double theta; //just for fun) (do NOT use this in a practical setting it will NOT work)
} hdCamera;

typedef struct hdScreen { 
	u16 size_x; 
	u16 size_y; //Important note: while this struct does store size_x and size_y, it is only updated during the draw call, so if you need screen size for anything else I cannot guarantee that the sizes will be accurate and you should use getmaxyx(). Treat these like private variables basically. 
	u32 count;
	u32 capacity; 
	hdSprite** items;
	int* collisionMap;
	hdCamera* camera;
} hdScreen;


#endif
