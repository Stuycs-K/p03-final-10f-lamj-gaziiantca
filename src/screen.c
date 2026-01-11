#include "types.h" 
#include "ascii_image.h" 

#include <ncurses.h>

void initScreen(){
	initscr(); 
	cbreak();
	noecho();
	keypad(stdscr, TRUE); 
	curs_set(0); 
	nodelay(stdscr, TRUE);
}

void drawSprite(hdScreen* screen, hdSprite* sprite){
	int x = sprite->pos_x;
	int y = sprite->pos_y;
	
}

void draw(hdScreen* screen){
	erase();
	for(int i=0; i<screen->count; i++){

	}
	refresh();
}
