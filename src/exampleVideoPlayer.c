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


hdCompressedImage* arrtocimg(u8* arr){
	printf("%d\n", arr[0]);
	hdCompressedImage* out = (hdCompressedImage*) (malloc(sizeof(hdCompressedImage)));
	u64 len = 0;
	printf("bro what\n");
	memcpy(&out->size_x, arr, sizeof(out->size_x));
	arr += 2;
	memcpy(&out->size_y, arr, sizeof(out->size_y));
	arr += 2;
	memcpy(&out->count, arr, sizeof(out->count));
	arr += 4;
	printf("vro %d\n", out->count);

	out->items = (hdCompressedPixel*) (malloc(sizeof(*out->items) * out->count));
	memcpy(out->items, arr, sizeof(*out->items) * out->count);
	arr += out->count * sizeof(*out->items);
	
	out->palette = (hdPixelPalette*) (calloc(sizeof(hdPixelPalette), 1));
	memcpy(&out->palette->count, arr, sizeof(out->palette->count));
	arr+=sizeof(out->palette->count);

	out->palette->items = (hdPixel*) (malloc(sizeof(*out->palette->items) * out->palette->count));
	memcpy(out->palette->items, arr, sizeof(*out->palette->items) * out->palette->count);
	
	return out;

}
void drawPixel(hdPixel* p){
	printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p->r, p->g, p->b, p->c); 
}

void drawImage(hdRawImage* img){
	for(int y=0; y<img->size_y; y++){
		for(int x=0; x<img->size_x; x++){
			hdPixel* p = &img->arr[y * img->size_x + x];
			drawPixel(p);
		}
		printf("\n");
	}
}

int main(int argc, char* argv[]){
	char* IP;
	if(argc == 1){
		IP = "127.0.0.1";
	}else{
		IP = argv[1];
	}
	struct addrinfo* results;
	hdNetwork* queue = initializeNetworkQueue(); 
	int sockfd = setupUDP_Client(IP, &results, queue); 
	hdPacket* out = createPacket("", 0); //This is used to let the server know we are still here
			Client_sendData(queue, out);
	hdPacket* in = (hdPacket*) (calloc(sizeof(hdPacket), 1));
	u64 time = 0;
	char* string = (char*) (malloc(128));
	hdScreen* screen = initScreen();
	hdSprite* imgSprite = initSprite(loadRawImage("assets/TheSkeld.txt"), NULL);
	addSprite(screen, imgSprite);
	while(1){
		if(getTime() - time > 100){
			time = getTime();
			Client_sendData(queue, out);
		}
		Client_receiveData(queue, &in); 
		if(*in->data == 0) continue;
		//hdCompressedImage* cimg = arrtocimg(in->data);
		//printf("%d\n",cimg->size_x);
		//hdRawImage* img = uncompressImage(cimg);
		//printf("%d\n",img->size_x);
		//drawImage(img);
		//printf("%d\n", *(int*)(in->data));
		//printf("%d\n", *(in->data+2));
		//imgSprite->image = img;
		//draw(screen);
		sprintf(string, "assets/cimg_apple/cimg_%05d.cimg", *(int*)(in->data));//i give up on making it work properly
		//printf("%s\n", string);
		hdCompressedImage* cimg = readCompressedImage(string, NULL);
		hdRawImage* img = uncompressImage(cimg);
		//hdRawImage* img = readRawImage(string);
		imgSprite->image = img;
		//drawImage(imgSprite->image);
		//printf("\n\n\n\n");
		usleep(10000);
		draw(screen);
	}
}
