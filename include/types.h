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
	u8 r; u8 g; u8 b; char c; 
	//please no padding mr c
} __attribute__((packed)) pixel; //NO PADDING MR GCC THANK YOU 

typedef struct rawImage {
	u16 size_x; 
	u16 size_y; 
	pixel* map; 
} rawImage;



#endif
