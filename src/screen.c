#include "types.h" 
#include "ascii_image.h"

#include <stdlib.h>
#include <stdio.h>

#include <ncurses.h>
#include <math.h>

//#define COLOR_CONVERTER_3000 3.92
//god I love ncurses

#define pointer_da_append(xs, x) \
	if(xs->count >= xs->capacity) {\
		xs->capacity = xs->capacity * 2 + 1;\
		xs->items = realloc(xs->items, xs->capacity*sizeof(x));\
	}\
	xs->items[xs->count++] = x;

hdScreen* initScreen(){
	hdScreen* out = (hdScreen*) (calloc(sizeof(hdScreen), 1));
	out->camera = (hdCamera*) (calloc(sizeof(hdCamera), 1));
	out->items = (hdSprite*) (malloc(0)); //divine intellect code don't question it
	initscr(); 
	cbreak();
	noecho();
	keypad(stdscr, TRUE); 
	curs_set(0); 
	nodelay(stdscr, TRUE);
	if(has_colors() == false){
		endwin();
		printf("Your terminal does not support colors!");
		exit(1);
	}
	start_color();
	for(int i=0; i<256; i++){
		init_pair(i, COLOR_WHITE, i);
	}
	return out;
}

int get_color(hdPixel p){
	//I am going to fucking lose it why does ncurses not support rgb normally??? 
	if(p.r == p.g && p.g == p.b){
		if(p.r < 8) return 16;
		if(p.r > 248) return 231; 
		return 232 + (int)((p.r-8) / 10.0);
	}
	int r6 = (p.r*5) / 255;
	int g6 = (p.g*5) / 255; 
	int b6 = (p.b*5) / 255;
	return 16 + (36 * r6) + (6 * g6) + b6;
}

void drawSprite(const hdScreen* screen, const hdSprite* sprite){
	i32 x = sprite->pos_x - screen->camera->pos_x;
	i32 y = sprite->pos_y - screen->camera->pos_y; //doing the rcs thingamabob for easy rotation formula
	/* 0,0 - 1,0
	 * |	  |
	 * 0,1 - 1,1
	 this thing^ */
	double theta = screen->camera->theta; 
	i32 l_x = screen->camera->pos_x - (screen->size_x / 2);
	i32 r_x = screen->camera->pos_x + (screen->size_x / 2);
	i32 t_y = screen->camera->pos_y - (screen->size_y / 2);
	i32 b_y = screen->camera->pos_y + (screen->size_y / 2); 
	//ncurses should handle this stuff automatically but still doing it just in case
	int pos_x, pos_y;
	hdPixel p;

	for(int i=0; i<sprite->image->size_y; i++){
		for(int j=0; j<sprite->image->size_x; j++){
			mvprintw(20, 0, "(%d %d)", pos_x, pos_y);
			if(theta != 0){
				pos_x = pos_x * cos(theta) - pos_y * sin(theta);
				pos_y = pos_y * sin(theta) + pos_y * cos(theta);
				
			}else{
				pos_x = x + j; 
				pos_y = y + i;
			}
			if(pos_x < l_x || pos_x > r_x || pos_y < t_y || pos_y > b_y) {
				continue;
			}
			p = sprite->image->arr[i * sprite->image->size_x + j];
			if(p.c == ' ') continue;
			//init_color(CUSTOM_COLOR, p.r * COLOR_CONVERTER_3000, p.g * COLOR_CONVERTER_3000, p.b * COLOR_CONVERTER_3000); //if only there was an easier way to do this :(
			//printf("(%hhu %hhu %hhu)\n", p.r, p.g, p.b);
			int cid = get_color(p);
			attron(COLOR_PAIR(cid));
			mvaddch(pos_y, pos_x, p.c); //this will lowkey print everything with an offset but that's tomorrow me's issue
			//mvprintw(pos_y, pos_x, "\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p.r, p.g, p.b, p.c);
			attroff(COLOR_PAIR(cid));
		}
	}
}

void draw(hdScreen* screen){
	erase();
	getmaxyx(stdscr, screen->size_x, screen->size_y);
	for(int i=0; i<screen->count; i++){
		drawSprite(screen, &screen->items[i]);
	}
	refresh();
}

void addSprite(hdScreen* screen, const hdSprite* sprite){
	pointer_da_append(screen, *sprite);
}

hdSprite* initSprite(hdRawImage* img, void* extra){
	hdSprite* out = (hdSprite*) (malloc(sizeof(hdSprite)));
	out->image = img; 
	out->pos_x = 0;
	out->pos_y = 0;
	out->extra = extra;
	return out;
}

void cleanupScreen(hdScreen* screen){
	endwin();
	free(screen->items);
	free(screen->camera);
	free(screen);
}

void nukeSprite(hdSprite* sprite){
	nukeRawImage(sprite->image);
	free(sprite->extra); 
	free(sprite);
}

void nukeScreen(hdScreen* screen){
	for(int i=0; i<screen->count; i++){
		nukeSprite(screen->items + i);
	}
	cleanupScreen(screen);
}
