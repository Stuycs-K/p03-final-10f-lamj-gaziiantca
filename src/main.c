#include <stdlib.h>
#include <stdio.h>

#include "ascii_image.h"

int main(){
	rawImage* img = loadRawImage("assets/testsprite.txt");
	for(int y=0; y<img->size_y; y++){
		for(int x=0; x<img->size_x; x++){
			pixel* p = &img->map[y * img->size_x + x];
			printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p->r, p->g, p->b, p->c); 
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p->r, p->g, p->b, p->c); 
		}
		printf("\n");
	}


	return 0;
}
