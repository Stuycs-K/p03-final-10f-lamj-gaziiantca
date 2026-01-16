#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include <ncurses.h>
#include <math.h>
#include "hashmap.h"

#include "engine_clock.h"
#include "player.h"
#include "ascii_image.h"
#include "event_signals.h"
#include "vector2.h"
#include "screen.h"
#include "networking.h"

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


void massProduceCompressedVideo(char* baseName, char* outName, int start, int end, bool canReusePallete){
	//for the bad apple videos, it will be assets/ascii_apple/ascii_
	char* fname = (char*) (malloc(sizeof(char) * 256));

	sprintf(fname, "%s%05d.txt", baseName, start);
	printf("%s\n", fname);
	hdRawImage* img = loadRawImage(fname); 
	hdCompressedImage* cimg = compressRawImage(img, NULL);
	start += 1;
	for(; start<end; start++){ 
		if(start%100 == 0){
			printf("%d\n", start);
		}
		sprintf(fname, "%s%05d.txt", baseName, start);
		hdRawImage* img = loadRawImage(fname); 
		hdCompressedImage* cimg = compressRawImage(img, NULL);
		sprintf(fname, "%s%05d.cimg", outName, start);
		writeCompressedImage(cimg, fname); 
	}
}

static void err2(const char* str, const char* str2){ 
	if(str != NULL){
		printf("%s", str); 
	}
	if(str2 != NULL){
		printf(" %s", str2); 
	}
	printf(" | %s\n", strerror(errno));
}


/*u8* cimgtoarr(hdCompressedImage* cimg, int* outSize){
	u8* arr = malloc(0);
	int len = sizeof(cimg->size_x) + sizeof(cimg->size_y) + sizeof(cimg->count);
	int pos = 0;
	arr = realloc(arr, len); 
	memcpy(arr, &cimg->size_x, sizeof(cimg->size_x));
	pos+=sizeof(cimg->size_x);
	memcpy(arr+pos, &cimg->size_y, sizeof(cimg->size_y));
	pos+=sizeof(cimg->size_y);
	memcpy(arr+pos, &cimg->count, sizeof(cimg->count)); //probably very inefficient. but also probably I don't care anymore
	//RELEASE ME AA]
	pos+=sizeof(cimg->count);
	len += cimg->count * sizeof(*cimg->items);
	arr = realloc(arr, len);
	memcpy(arr+pos, cimg->items, cimg->count * sizeof(*cimg->items));
	pos += cimg->count * sizeof(*cimg->items);

	len += sizeof(cimg->palette->count);
	arr = realloc(arr, len);
	memcpy(arr+pos, &cimg->palette->count, sizeof(cimg->palette->count));
	pos += sizeof(cimg->palette->count);

	len += cimg->palette->count * sizeof(*cimg->palette->items);
	arr = realloc(arr, len); 
	memcpy(arr+pos, cimg->palette->items, cimg->palette->count * sizeof(*cimg->palette->items));



	*outSize = len;
	return arr;
}*/
u8* cimgtoarr(hdCompressedImage* cimg, int* outSize){
	u8* arr = malloc(0);
	int len = sizeof(cimg->size_x) + sizeof(cimg->size_y) + sizeof(cimg->count);
	int pos = 0;
	arr = realloc(arr, len); 
	memcpy(arr, &cimg->size_x, sizeof(cimg->size_x));
	pos += 2;
	memcpy(arr+pos, &cimg->size_y, sizeof(cimg->size_y));
	pos += 2;
	memcpy(arr+pos, &cimg->count, sizeof(cimg->count)); //probably very inefficient. but also probably I don't care anymore
	//RELEASE ME AA]
	pos += 4;
	len += cimg->count * sizeof(*cimg->items);
	arr = realloc(arr, len);
	memcpy(arr+pos, cimg->items, cimg->count * sizeof(*cimg->items));
	pos += cimg->count * sizeof(*cimg->items);

	len += sizeof(cimg->palette->count);
	arr = realloc(arr, len);
	memcpy(arr+pos, &cimg->palette->count, sizeof(cimg->palette->count));
	pos += sizeof(cimg->palette->count);

	len += cimg->palette->count * sizeof(*cimg->palette->items);
	arr = realloc(arr, len); 
	memcpy(arr+pos, cimg->palette->items, cimg->palette->count * sizeof(*cimg->palette->items));



	*outSize = len;
	return arr;
}

int main(){
	//massProduceCompressedVideo("assets/ascii_apple/ascii_", "assets/cimg_apple/cimg_", 0, 6562, false);

	hdNetwork* network = initializeNetworkQueue(); 
	int server_fd = setupUDP_Server(network); 
	hdPacket* buffer = (hdPacket*)(calloc(sizeof(hdPacket),1));
	hdPacket* out;
	//hdCompressedImage* cimg = readCompressedImage("assets/cimg_apple/cimg_00134.cimg", NULL);
	int i=0;
	int* data = &i;
	while(i<6500){
		i++;
		Server_receiveData(network, &buffer);
		//int len;
		//u8* data = cimgtoarr(cimg, &len); i give up bruh

		out = createPacket(data, sizeof(data));
		Server_broadcastData(network, out);
		//printf("Sus\n");
		usleep(1000000 / 30);
	}


}
