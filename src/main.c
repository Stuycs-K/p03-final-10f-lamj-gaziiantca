#include <stdlib.h>
#include <stdio.h>

#include <ncurses.h>
#include "hashmap.h"

#include "engine_clock.h"
#include "player.h"
#include "ascii_image.h"


void testRawImageReadingAndWriting(char* path){
	hdRawImage* img = loadRawImage(path);
	for(int y=0; y<img->size_y; y++){
		for(int x=0; x<img->size_x; x++){
			hdPixel* p = &img->arr[y * img->size_x + x];
			printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p->r, p->g, p->b, p->c); 
		}
		printf("\n");
	}
	writeRawImage(img, "temp.img"); 
	hdRawImage* img2 = readRawImage("temp.img");
	for(int y=0; y<img->size_y; y++){
		for(int x=0; x<img->size_x; x++){
			hdPixel* p1 = &img2->arr[y * img->size_x + x];
			hdPixel* p2 = &img->arr[y * img->size_x + x];
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

void testRawImageCompression(char* path){
	hdRawImage* img = loadRawImage(path);
	hdCompressedImage* cimg = compressRawImage(img, NULL);
}

void testHashing(){
	//printf("A\n");
	hdPixel* p1 = (hdPixel*)(calloc(sizeof(hdPixel), 1)); 
	hdPixel* p2 = (hdPixel*)(calloc(sizeof(hdPixel), 1)); 
	hdPixel* p3 = (hdPixel*)(calloc(sizeof(hdPixel), 1)); 
	p1->c = 'a'; p1->r = 100; p1->g = 100; p1->b = 100; 
	p2->c = 'a'; p2->r = 100; p2->g = 100; p2->b = 100; 
	p3->c = 'a'; p3->r = 100; p3->g = 99; p3->b = 100; 
	//printf("A\n");
	struct hashmap *map = hashmap_new(sizeof(hdHashEntry), 0, 0, 0, pixel_hash, pixel_compare, NULL, NULL);
	//printf("A\n");
	hashmap_set(map, &(hdHashEntry){ .pixel = p1, .pos = 1});
	hdHashEntry* result;
	result = (hdHashEntry*) hashmap_get(map, &(hdHashEntry) {.pixel = p1 });
	printf("Position of p1 when it is the only element with a value of 1. This should print 1: %d\n", result->pos);
	if(result->pos != 1){
		return;
	}
	result = (hdHashEntry*) hashmap_get(map, &(hdHashEntry) {.pixel = p2 });
	printf("p2, an identical pixel to p1's position. This, too, should print 1: %d\n", result->pos);
	
	if(result->pos != 1){
		return;
	}
	result = (hdHashEntry*) hashmap_get(map, &(hdHashEntry) {.pixel = p3 });
	printf("p3==0, a different pixel that's not in the map yet. This, as well, should print 1: %d\n", result==0);
	if(result != 0){
		return;
	}
	
	
	

}


void exitGame(int n){
	curs_set(1);
	endwin();
	exit(n);
}

int get_wasd_input() {
  int ch = getch();
  if (
    ch == 'w' || ch == 'W' || 
    ch == 'a' || ch == 'A' || 
    ch == 's' || ch == 'S' || 
    ch == 'd' || ch == 'D') {
      return ch;
  }

  return 0;
}

void testEngineClock() {
  EngineClock_init();

  Player playerStruct;
  Player* newPlayer = &playerStruct;
  Player_init(newPlayer, "Aleksandr");

  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  curs_set(0);

  // double timeToElapse = 1 - .001f;
  // while (timeElapsed() < timeToElapse) {
  while (1) {
    double dt = waitForNextFrame();
    char input = get_wasd_input();
    Player_handleInput(newPlayer, input);
    Player_updateMovement(newPlayer, dt);

    erase();
    mvprintw(0, 0, "Time Elapsed: %.2lfs", timeElapsed());
    mvprintw(1, 0, "Pos: (%.2lf, %.2lf)", newPlayer->pos.x, newPlayer->pos.y);
    mvprintw(2, 0, "dt=%lf | TPS=%.2f\n", dt, 1/dt);
    mvprintw(3,0, "Current input: %c", input);
    refresh();
  }

  exitGame(0);
}

int main(){
	//testRawImageReadingAndWriting("assets/sus.txt");
	testRawImageCompression("assets/smallsus.txt");
	//testHashing();
	
  //testEngineClock();
}
