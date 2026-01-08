#include <stdlib.h>
#include <stdio.h>

#include "ascii_image.h"

void testRawImageReadingAndWriting(char* path){
	hdRawImage* img = loadRawImage(path);
	for(int y=0; y<img->size_y; y++){
		for(int x=0; x<img->size_x; x++){
			hdPixel* p = &img->map[y * img->size_x + x];
			printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p->r, p->g, p->b, p->c); 
		}
		printf("\n");
	}
	writeRawImage(img, "temp.img"); 
	hdRawImage* img2 = readRawImage("temp.img");
	for(int y=0; y<img->size_y; y++){
		for(int x=0; x<img->size_x; x++){
			hdPixel* p1 = &img2->map[y * img->size_x + x];
			hdPixel* p2 = &img->map[y * img->size_x + x];
			printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p1->r, p1->g, p1->b, p1->c); 
			printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p2->r, p2->g, p2->b, p2->c); 
			if(p1->r != p2->r){
				printf("sus (%d,%d) %d vs %d\n", x, y, p1->r, p2->r);
				return;
			}
		}
		printf("\n");
	}
	remove("temp.img");
	printf("it did not break :thumbsup:\n");


}


int main(){
	/*hdRawImage* img = loadRawImage("assets/testsprite.txt");
	for(int y=0; y<img->size_y; y++){
		for(int x=0; x<img->size_x; x++){
			hdPixel* p = &img->map[y * img->size_x + x];
			printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p->r, p->g, p->b, p->c); 
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p->r, p->g, p->b, p->c); 
		}
		printf("\n");
	}
	writeRawImage(img, "kneesurgery.img");
*/
	testRawImageReadingAndWriting("assets/sus.txt");
	return 0;
}
