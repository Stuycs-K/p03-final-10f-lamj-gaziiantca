#include <stdlib.h>
#include <stdio.h>

#include <ncurses.h>

#include "engine_clock.h"
#include "player.h"
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

    clear();
    mvprintw(0, 0, "Time Elapsed: %.2lfs", timeElapsed());
    mvprintw(1, 0, "Pos: (%.2lf, %.2lf)", newPlayer->pos.x, newPlayer->pos.y);
    mvprintw(2, 0, "dt=%lf | TPS=%.2f\n", dt, 1/dt);
    mvprintw(3,0, "Current input: %c", input);
    refresh();
  }

  exitGame(0);
}

int main(){
	// loadRawImage("assets/testsprite.txt");
  testEngineClock();
}
