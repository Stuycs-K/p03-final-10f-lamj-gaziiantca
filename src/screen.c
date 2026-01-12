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
	//why does ncurses not support rgb normally??? why are we still here??? just to suffer??????
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
	i32 x = sprite->pos_x + screen->camera->pos_x;
	i32 y = sprite->pos_y + screen->camera->pos_y;
	//should probably do smth like this:
	/* -1,-1 === 1,-1
	 * |    0,0   |
	 * -1,1 ===  1,1
	 this thing^ */
	double theta = screen->camera->theta; 
	//camera is on the top right
	i32 l_x = screen->camera->pos_x;
	i32 r_x = screen->camera->pos_x + (screen->size_x);
	i32 t_y = screen->camera->pos_y;
	i32 b_y = screen->camera->pos_y + (screen->size_y); 
	
	i32 c_x = screen->camera->pos_x + screen->size_x / 4;
	i32 c_y = screen->camera->pos_y + screen->size_y / 4;

	//ncurses should handle this stuff automatically but still doing it so that we don't draw things off-screen
	int pos_x, pos_y;
	hdPixel p;
	int i = 0; int sj = 0;
	int mi = sprite->image->size_y; 
	int mj = sprite->image->size_x; 
	/*if(l_x > x) sj = x;
	if(r_x <= x+mj) mj = r_x+x; //mj = mj-(x-r_x+mj);	
	if(t_y > y) i = t_y-y;
	if(b_y < y+mi) mi = b_y-y; commenting this out is a very temporary fix, and by temporary...welll..let's just stay it's staying that way*/ 
	
	if(mj < sj || mj < 0 || mi < i || i < 0) return;

	mvprintw(20, 0, "(%d %d - %d %d) [%d %d %d %d] [%d-%d %d-%d] over [%d %d]", x, y, x + sprite->image->size_x, y + sprite->image->size_y, l_x, r_x, t_y, b_y, i, mi, sj, mj, c_x, c_y);
	for(; i<mi; i++){
		for(int j=sj; j<mj; j++){
			pos_x = x+j;
			pos_y = x+i;
			if(theta != 0){
				pos_x -= screen->size_x;
				pos_y -= screen->size_y;
				pos_x = pos_x * cos(theta) - pos_y * sin(theta);
				pos_y = pos_x * sin(theta) + pos_y * cos(theta);
				pos_x += screen->size_x;
				pos_y += screen->size_y;

			}
			/*if(pos_x < l_x || pos_x > r_x || pos_y < t_y || pos_y > b_y) {
				continue; 
			}*/
			p = sprite->image->arr[i * sprite->image->size_x + j];
			if(p.c == ' ') continue;
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
