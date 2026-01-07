#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "ascii_image.h"

#define da_append(xs, x) \
	if(xs.count >= xs.capacity) {\
		xs.capacity = xs.capacity * 2 + 1;\
		xs.items = realloc(xs.items, xs.capacity*sizeof(x));\
	}\
	xs.items[xs.count++] = x;
//could maybe be funny to build an arena allocator for this but idk if it matters enough 


typedef struct dFragArr {
	hdPixel* items; 
	u32 count; 
	u32 capacity;
} dFragArr; 

void err(char* str){
	if(str != NULL){
		printf("%s", str); 
	}
	printf(" | %s\n", strerror(errno));
}

hdRawImage* loadRawImage(char* path){ //important note: i will not make this run fast because this is something that should be run once ever per image. whatever game this is used for should not call this. this is a tool for development. not for game. ok.
	hdRawImage* out = (hdRawImage*)(malloc(sizeof(hdRawImage)));
	
	FILE* f = fopen(path, "r");
	if(f == 0) err("loadRawImage fopen");
	char* buf = (char*)(calloc(sizeof(char), 200001)); //there's probably a better way to do this
	int p, q = 0; 
	hdPixel* stamp = (hdPixel*)(calloc(sizeof(hdPixel), 1));
	int size_x = 0; int size_y = 0;
	u8 xFlag = 1; 
	u8 fortyeightFlag = 0;
	dFragArr f_arr = {0};
	while(fgets(buf, 200000, f) != NULL){
		size_y++; 
		p = 0;
		while(buf[p] != 0){
			if(buf[p] == 27){ //the year is 2057. I still have yet to figure out how to write the ascii character number 27 without just using the number. 
				q = p; 
				while(buf[q] != 0 && buf[q] != 'm'){
					q++;
				}
				if(buf[q] == 'm'){
					if(!fortyeightFlag){
						sscanf(buf+p+1, "[38;2;%hhu;%hhu;%hhum%c\[[0m", &stamp->r, &stamp->g, &stamp->b, &stamp->c);
						if(stamp->c == 0){
							fortyeightFlag = 1;
							goto fortyeight;
						}
					}else{
fortyeight:
						sscanf(buf+p+1, "[48;2;%hhu;%hhu;%hhum%c\[[0m", &stamp->r, &stamp->g, &stamp->b, &stamp->c);
					}
					//printf("%c", stamp->c);
					da_append(f_arr, *stamp);
				}
				p+=(q-p)+4; //god I love string formatting. this makes sure that the loop skips the ansi reset sequence
				if(xFlag == 1){
					size_x++;
				}
			}
			p++;
		}
		//printf("\n");
		if(xFlag == 1){
			xFlag = 0; 
		}
	}
	out->map = f_arr.items; 
	out->size_x = size_x; 
	out->size_y = size_y; 
#ifdef NDEBUG
	printf("Loaded a texture of size %d X %d with %d total fragments\n", size_x, size_y, f_arr.count); 
#endif
	return out;
}

void writeRawImage(hdRawImage* img, char* path) {
	int f = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0644); 
	int bytes; 
	bytes = write(f, &img->size_x, sizeof(img->size_x));
	if(bytes < 0) err("Writing raw image size x");
	bytes = write(f, &img->size_y, sizeof(img->size_y));
	if(bytes < 0) err("Writing raw image size y");
	bytes = write(f, &img->map, img->size_x * img->size_y * sizeof(*img->map)); //boy I sure do hope my array doesn't decay! (he works at the array decay factory)
	if(bytes < 0) err("Writing raw image map");
}

