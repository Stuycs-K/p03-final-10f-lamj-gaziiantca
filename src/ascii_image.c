#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "ascii_image.h"
#include "hashmap.h"

#define da_append(xs, x) \
	if(xs.count >= xs.capacity) {\
		xs.capacity = xs.capacity * 2 + 1;\
		xs.items = realloc(xs.items, xs.capacity*sizeof(x));\
	}\
	xs.items[xs.count++] = x;
//could maybe be funny to build an arena allocator for this but idk if it matters enough 

#define pointer_da_append(xs, x) \
	if(xs->count >= xs->capacity) {\
		xs->capacity = xs->capacity * 2 + 1;\
		xs->items = realloc(xs->items, xs->capacity*sizeof(x));\
	}\
	xs->items[xs->count++] = x;

typedef struct dFragArr {
	hdPixel* items; 
	u32 count; 
	u32 capacity;
} dFragArr; 

void err(const char* str){
	if(str != NULL){
		printf("%s", str); 
	}
	printf(" | %s\n", strerror(errno));
}
void err2(const char* str, const char* str2){ 
	if(str != NULL){
		printf("%s", str); 
	}
	if(str2 != NULL){
		printf(" %s", str2); 
	}
	printf(" | %s\n", strerror(errno));
}

u8 writeReadDebug = 0;

hdRawImage* loadRawImage(const char* path){ //important note: i will not make this run reasonably fast because this is something that should be run once ever per image. whatever game this is used for should not call this. this is a tool for development. not for game. 
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
	out->arr = f_arr.items; 
	out->size_x = size_x; 
	out->size_y = size_y; 
#ifdef NDEBUG
	printf("Loaded a texture of size %d X %d with %d total fragments\n", size_x, size_y, f_arr.count); 
#endif
	return out;
}

void writeRawImage(const hdRawImage* img, const char* path) {
	int f = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0644); 
	if(f < 0) err(path);
	int bytes; 
#ifdef NDEBUG
	printf("Writing sizes %d X %d\n", img->size_x, img->size_y);
#endif
	bytes = write(f, &img->size_x, sizeof(img->size_x));
	if(bytes < 0) err("Writing raw image size x");
	bytes = write(f, &img->size_y, sizeof(img->size_y));
	if(bytes < 0) err("Writing raw image size y");
	bytes = write(f, img->arr, img->size_x * img->size_y * sizeof(*img->arr)); //boy I sure do hope my array doesn't decay! (he works at the array decay factory)
	if(bytes < 0) err("Writing raw image map");
#ifdef NDEBUG
		printf("Wrote %d bytes map, should be %lu\n", bytes, img->size_x * img->size_y * sizeof(hdPixel));
#endif
	}

hdRawImage* readRawImage(const char* path){
	int f = open(path, O_RDONLY, 0); 
	if(f < 0) err(path);
	int bytes;  
	hdRawImage* out = (hdRawImage*)(malloc(sizeof(hdRawImage)));

	bytes = read(f, &out->size_x, sizeof(out->size_x));
	if(bytes < 0) err("Reading raw image size x");
	bytes = read(f, &out->size_y, sizeof(out->size_y));
	if(bytes < 0) err("Reading raw image size y");
#ifdef NDEBUG
	printf("Trying to read %lu byte %d X %d map\n", out->size_x * out->size_y * sizeof(*out->arr), out->size_x, out->size_y);
#endif
	out->arr = (hdPixel*) (malloc(out->size_x * out->size_y * sizeof(*out->arr)));
	bytes = read(f, out->arr, out->size_x * out->size_y * sizeof(*out->arr));
	if(bytes < 0) err("Reading raw image map");

	return out;
}

void nukeRawImage(hdRawImage* img){
	free(img->arr); 
	free(img);
}

int pixel_compare(const void* a, const void* b, void *udata){
	const hdHashEntry* p1 = a;
	const hdHashEntry* p2 = b; 
	return memcmp(p1->pixel, p2->pixel, sizeof(hdPixel));
}

uint64_t pixel_hash(const void* item, u64 seed0, u64 seed1){
	const hdHashEntry* p = item; 
	//printf("(%hhu, %hhu, %hhu, %c) / %b -> %lu\n", p->pixel->r, p->pixel->g, p->pixel->b, p->pixel->c, *p->pixel, hashmap_sip(p->pixel, sizeof(*p->pixel), seed0, seed1));
	return hashmap_sip(p->pixel, sizeof(*p->pixel), seed0, seed1);
}

int addPixelToSymtab(hdHashEntry* in, const hdCompressedImage* img){ //in has to be a pointer for the hash get :( 
	const hdHashEntry* result; 
	hdPixel p2 = *in->pixel;
	//printf("symtab adding (%hhu, %hhu, %hhu, %c)\n", p2.r, p2.g, p2.b, p2.c);
	//printf("sus %p\n", img->palette);
	result = (hdHashEntry*) hashmap_get(img->palette->hashmap, in);
	//printf("%p\n", result);
	if(result != NULL){
		//printf("Symtab already has the thing! how convenient\n");
		return result->pos;
	}else{
		in->pos = img->palette->count;
		pointer_da_append(img->palette, *in->pixel);
		//printf("%d\n", *(in->pixel));
		/*if(img->palette->count >= img->palette->capacity){
			img->palette->capacity = img->palette->capacity * 2 + 1; 
			img->palette->items = (hdPixel*) (realloc(img->palette->items, img->palette->capacity * sizeof(hdPixel)));
		}
		img->palette->items[in->pos] = *in->pixel;
		img->palette->count++;*/


		hashmap_set(img->palette->hashmap, in);
		return in->pos;
	}
}

#define pixelCmp(p1, p2) (p1.r==p2.r && p1.g==p2.g && p1.b==p2.b && p1.c==p2.c)

hdCompressedImage* compressRawImage(const hdRawImage* img, hdPixelPalette* palette){
	hdCompressedImage* out = (hdCompressedImage*) (calloc(sizeof(hdCompressedImage), 1));
	out->size_x = img->size_x;
	out->size_y = img->size_y; 
	if(palette == NULL){
		palette = (hdPixelPalette*) (calloc(sizeof(hdPixelPalette), 1));
		palette->hashmap = hashmap_new(sizeof(hdHashEntry), 0, 0, 0, pixel_hash, pixel_compare, NULL, NULL);
		palette->items = (calloc(sizeof(*palette->items), 1));
		palette->capacity = 0; palette->count = 0;
	}
	out->palette = palette;
	hdPixel p1 = img->arr[0];
	hdPixel p2;
	hdCompressedPixel stamp = { .count = 0};
	hdHashEntry* entry = &(hdHashEntry) { .pixel = &p1 };
	stamp.pos = addPixelToSymtab(entry, out);
	for(int y=0; y<out->size_y; y++){
		for(int x=0; x<out->size_x; x++){
			p2 = img->arr[y * img->size_x + x];
			if(!pixelCmp(p1, p2)){
				p1 = p2;
				pointer_da_append(out, stamp);
				stamp.count = 0;
				entry->pixel = &p2;
				stamp.pos = addPixelToSymtab(entry, out);
			}
			stamp.count++;
			if(stamp.count > 60000){
				printf("Stamp count in sus territories\n");
			}
			if(palette->count > 1000000000){
				printf("palette items in sus territories\n");
			}
			//printf("%d\n", stamp.count);
			//printf("palette item at stamp pos (count: %d) (%hhu, %hhu, %hhu, %c), alt rep: %d\n", stamp.count, palette->items[stamp.pos].r, palette->items[stamp.pos].g, palette->items[stamp.pos].b, palette->items[stamp.pos].c, palette->items[stamp.pos]);
			//printf("Curr pixel (count: %d) (%hhu, %hhu, %hhu, %c)\n", stamp.count, p2.r, p2.g, p2.b, p2.c);
		}
	}
	
	return out;
	
}

hdRawImage* uncompressImage(const hdCompressedImage* img){
	hdRawImage* out = (hdRawImage*) (malloc(sizeof(hdRawImage)));
	out->size_x = img->size_x;
	out->size_y = img->size_y;
	out->arr = (hdPixel*) (malloc(sizeof(hdPixel) * out->size_x * out->size_y));
	dFragArr f_arr = {0};

	for(int i=0; i<img->count; i++){
		for(int j=0; j<img->items[i].count; j++){
			da_append(f_arr, img->palette->items[img->items[i].pos]);
		}
	}
	out->arr = f_arr.items;
	return out;
}

void writeCompressedImage(const hdCompressedImage* img, const char* path){
	int f = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(f < 0) err(path);
	int bytes;
	int tbytes = 0;

	bytes = write(f, &img->size_x, sizeof(img->size_x));
	if(bytes < 0) err2("Writing compressed image size x", path);
	tbytes += bytes;

	bytes = write(f, &img->size_y, sizeof(img->size_y));
	if(bytes < 0) err2("Writing compressed image size y", path);
	tbytes += bytes;

	bytes = write(f, &img->count, sizeof(img->count));
	if(bytes < 0) err2("Writing compressed image count", path); 
	tbytes += bytes;

	bytes = write(f, img->items, sizeof(*img->items) * img->count);
	if(bytes < 0 || writeReadDebug) {
		if(writeReadDebug) printf("%d\n", *img->items);
		char* errstr = (char*)(malloc(200)); 
		sprintf(errstr, "Writing compressed image arr %p of size %d, %d bytes", img->items, img->count, bytes);
		err(errstr);
	}
	tbytes += bytes;

	bytes = write(f, &img->palette->count, sizeof(img->palette->count)); 
	if(bytes < 0) err2("Writing compresed image palette count", path); 
	tbytes += bytes;
	
	bytes = write(f, img->palette->items, sizeof(*img->palette->items) * img->palette->count);
	if(bytes < 0 || writeReadDebug) {
		if(writeReadDebug) printf("%d\n", *img->palette->items);
		char* errstr = (char*)(malloc(200)); 
		sprintf(errstr, "Writing palette arr %p of size %d, %d bytes", img->palette->items, img->palette->count, bytes);
		err(errstr);
	}
	tbytes += bytes;

	close(f);
	if(writeReadDebug) printf("Wrote %d bytes\n", tbytes);
}

hdCompressedImage* readCompressedImage(const char* path, hdPixelPalette* palette){ //palette arg just for fun yk
	hdCompressedImage* out = (hdCompressedImage*) (malloc(sizeof(hdCompressedImage)));
	int f = open(path, O_RDONLY, 0);
	if(f < 0) err(path); 
	int bytes;
	int tbytes = 0;

	bytes = read(f, &out->size_x, sizeof(out->size_x));
	if(bytes < 0) err2("Reading size_x of compressed image", path);
	tbytes += bytes;

	bytes = read(f, &out->size_y, sizeof(out->size_y));
	if(bytes < 0) err2("Reading size_y of compressed image", path);
	tbytes += bytes;

	bytes = read(f, &out->count, sizeof(out->count));
	if(bytes < 0) err2("Reading count of compressed image", path);
	tbytes += bytes;


	out->items = (hdCompressedPixel*) (malloc(sizeof(*out->items) * out->count));
	bytes = read(f, out->items, sizeof(*out->items) * out->count);
	if(bytes < 0 || writeReadDebug) {
		if(writeReadDebug) printf("%d\n", *out->items);
		char* errstr = (char*)(malloc(200)); 
		sprintf(errstr, "Reading compressed image arr %p of size %d, %d bytes", out->items, out->count, bytes);
		err(errstr);
	}
	tbytes += bytes;

	if(palette != NULL){
		out->palette = palette;
	}else{
		out->palette = (hdPixelPalette*) (calloc(sizeof(hdPixelPalette), 1)); //zero-d out just in case

		bytes = read(f, &out->palette->count, sizeof(out->palette->count));
		if(bytes < 0) err2("Reading compressed image palette count", path);

		tbytes += bytes;

		out->palette->items = (hdPixel*)(malloc(sizeof(*out->palette->items) * out->palette->count));
		bytes = read(f, out->palette->items, sizeof(*out->palette->items) * out->palette->count);
		if(bytes < 0 || writeReadDebug) {
			if(writeReadDebug) printf("%d\n", *out->palette->items);
			char* errstr = (char*)(malloc(200)); 
			sprintf(errstr, "Reading palette arr %p of size %d, %d bytes", out->palette->items, out->palette->count, bytes); //ok the bytes will always be -1 but just remove the if statement to see this info,, 
			err(errstr);
		}
		tbytes += bytes;
	}
	if(writeReadDebug) printf("Read %d bytes\n", tbytes);
	close(f);
	return out;
	
}

void nukePalette(hdPixelPalette* palette){
	hashmap_free(palette->hashmap);
	free(palette->items);
	free(palette);
}

void cleanupCompressedImage(hdCompressedImage* img){ //notably doesn't nuke the palette. 
	free(img->items);
	free(img);
}

void nukeCompressedImage(hdCompressedImage* img){
	nukePalette(img->palette); 
	cleanupCompressedImage(img);
}
