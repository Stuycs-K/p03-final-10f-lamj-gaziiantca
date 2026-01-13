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
	printf("compressing image\n");
	u64 time; 
	time = getTime();
	hdCompressedImage* cimg = compressRawImage(img, NULL);
	printf("Compressing image took %lu ms\n", (getTime() - time) / 1000);
	printf("uncompressing image\n");
	time = getTime();
	hdRawImage* img2 = uncompressImage(cimg);
	printf("uncompressing image took %lu ms\n", (getTime() - time) / 1000);

	printf("comparing the images\n");
	for(int y=0; y<img2->size_y; y++){
		for(int x=0; x<img2->size_x; x++){
			hdPixel* p1 = &img2->arr[y * img->size_x + x];
			hdPixel* p2 = &img->arr[y * img->size_x + x];
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p1->r, p1->g, p1->b, p1->c); 
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p2->r, p2->g, p2->b, p2->c); 
			if(p1->r != p2->r){
				printf("sus (%d,%d) %d vs %d\n", x, y, p1->r, p2->r);
				return;
			}
		}
		//printf("\n");
	}
	printf("Writing the image\n");
	writeCompressedImage(cimg, "temp.compimg");
	printf("Reading the image\n");

	time = getTime();
	hdCompressedImage* cimg2 = readCompressedImage("temp.compimg", NULL);
	printf("Reading took %lu ms\n", (getTime()-time) / 1000);
	
	printf("Uncompressing the read image\n");
	hdRawImage* img3 = uncompressImage(cimg2);
	printf("Comparing the image after reading with the original image\n");
	for(int y=0; y<img3->size_y; y++){
		for(int x=0; x<img3->size_x; x++){
			hdPixel* p1 = &img3->arr[y * img->size_x + x];
			hdPixel* p2 = &img->arr[y * img->size_x + x];
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p1->r, p1->g, p1->b, p1->c); 
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p2->r, p2->g, p2->b, p2->c); 
			if(p1->r != p2->r){
				printf("sus (%d,%d) %d vs %d\n", x, y, p1->r, p2->r);
				return;
			}
		}
		//printf("\n");
	}
	printf("Now reading the image while re-using the palette from the old image\n");

	time = getTime();
	hdCompressedImage* cimg3 = readCompressedImage("temp.compimg", cimg->palette);
	printf("Reading took %lu ms\n", (getTime() - time) / 1000);

	printf("Comparing uncompressed with original\n"); 
	hdRawImage* img4 = uncompressImage(cimg3);
	for(int y=0; y<img4->size_y; y++){
		for(int x=0; x<img4->size_x; x++){
			hdPixel* p1 = &img4->arr[y * img->size_x + x];
			hdPixel* p2 = &img->arr[y * img->size_x + x];
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p1->r, p1->g, p1->b, p1->c); 
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p2->r, p2->g, p2->b, p2->c); 
			if(p1->r != p2->r){
				printf("sus (%d,%d) %d vs %d\n", x, y, p1->r, p2->r);
				return;
			}
		}
		//printf("\n");
	}

	nukeRawImage(img); 
	nukeRawImage(img2);
	nukeRawImage(img3);
	nukeRawImage(img4);
	
	nukeCompressedImage(cimg); //this will run nukePalette(cimg->palette)
	nukeCompressedImage(cimg2); 
	cleanupCompressedImage(cimg3); //Since cimg3 was created by reusing the palette from cimg, if I were to run nuke I would free the palette twice, which is undefined behaviour. Therefore I use cleanupCompressedImage which does NOT free the palette.

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

typedef struct { 
  int var; 
  Vector2 lastPos;
  Connection* self;
} MovedCtx;

void testEvent(void* context, void* args) { // Check if player x pos is >= 10
  MovedCtx* con = (MovedCtx*) context;
  Vector2 pos = *((Vector2*) args);

  Vector2 delta = Vector2_sub(pos, con->lastPos);
  con->lastPos = pos;

  if (Vector2_mag(delta) > 0) {
    mvprintw(4, 0, "Position changed by (%.3lf, %.3lf)", delta.x, delta.y);
  }

  if (pos.x >= 10) {
    con->var = 1;
    Connection_Disconnect(con->self);
  }
}

void testBudgetGameLoop() {
  EngineClock_init();

  Player playerStruct;
  Player* newPlayer = &playerStruct;
  Player_init(newPlayer, "Aleksandr");

  MovedCtx* context = calloc(1, sizeof(MovedCtx));
  context->self = Signal_Connect(newPlayer->moved, &testEvent, context);

  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  curs_set(0);

  while (1) {
    clear();
    double dt = EngineClock_waitForNextFrame();
    char input = get_wasd_input();
    Player_handleInput(newPlayer, input);
    Player_updateMovement(newPlayer, dt);

    mvprintw(0, 0, "Time Elapsed: %.2lfs", EngineClock_getTimeElapsed());
    mvprintw(1, 0, "Pos: (%.2lf, %.2lf)", newPlayer->pos.x, newPlayer->pos.y);
    mvprintw(2, 0, "dt=%lf | TPS=%.2f\n", dt, 1/dt);
    mvprintw(3, 0, "Current input: %c", input);

    if (context->var) {
      mvprintw(5, 0, "Player x position has passed 10!!");
    }

    refresh();
  }

  exitGame(0);
}


void testScreen(char* path1, char* path2){
	EngineClock_init(); 
	Player* newPlayer = (Player*) calloc(1, sizeof(Player)); 
	Player_init(newPlayer, "Jesse");

  MovedCtx* context = calloc(1, sizeof(MovedCtx));
  context->self = Signal_Connect(newPlayer->moved, &testEvent, context);

	hdScreen* screen = initScreen();
	hdSprite* bg = initSprite(loadRawImage(path2), NULL);
	hdSprite* amog = initSprite(loadRawImage(path1), NULL);
	addSprite(screen, bg);
	addSprite(screen, amog);
	while(1){
		double dt = EngineClock_waitForNextFrame();
		char input = get_wasd_input(); 
		Player_handleInput(newPlayer, input);
		Player_updateMovement(newPlayer, dt*75);
		screen->camera->pos_x = round(newPlayer->pos.x);
		screen->camera->pos_y = round(newPlayer->pos.y);
		//screen->camera->theta = M_PI / 2;

		draw(screen);
		mvprintw(10, 0, "Pos: (%.2lf, %.2lf)", newPlayer->pos.x, newPlayer->pos.y);
    if (context->var) {
      mvprintw(11, 0, "Player x position has passed 10!!");
    }
		refresh();
	}
}

int main(){
	//testRawImageReadingAndWriting("assets/sus.txt");
	//testRawImageCompression("assets/big.texture");
	//testHashing();
  //testEngineClock();
	testScreen("assets/TheSkeld.txt", "assets/sus.txt");
  // testBudgetGameLoop();
}
