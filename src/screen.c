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
	i32 x = sprite->pos_x - screen->camera->pos_x;
	i32 y = sprite->pos_y + screen->camera->pos_y; //doing the rcs thingamabob for easy rotation formula
	/* -1,-1 === 1,-1
	 * |    0,0   |
	 * -1,1 ===  1,1
	 this thing^ */
	double theta = screen->camera->theta; 
	i32 l_x = screen->camera->pos_x;
	i32 r_x = screen->camera->pos_x + (screen->size_x);
	i32 t_y = screen->camera->pos_y;
	i32 b_y = screen->camera->pos_y + (screen->size_y); 
	//ncurses should handle this stuff automatically but still doing it just in case
	int c_x = l_x + r_x / 2;
	int c_y = t_y + b_y / 2;
	double pos_x, pos_y, pos2_x, pos2_y;
	hdPixel p;
	int i = 0; int sj = 0;
	int mi = sprite->image->size_y; 
	int mj = sprite->image->size_x; 
	//if(l_x > x) sj = l_x-x;
	//if(r_x <= x+mj) mj = r_x-x; //mj = mj-(x-r_x+mj);	
	//if(t_y > y) i = t_y-y;
	//if(b_y < y+mi) mi = b_y-y;
	
	//if(mj < sj || mj < 0 || mi < i || i < 0) return;

	//mvprintw(20, 0, "(%d %d - %d %d) [%d %d %d %d] [%d-%d %d-%d]", x, y, x + sprite->image->size_x, y + sprite->image->size_y, l_x, r_x, t_y, b_y, i, mi, sj, mj);
	for(; i<mi; i++){
		for(int j=sj; j<mj; j++){
				pos_x = x + j;
				pos_y = y + i;
				if(theta != 0){
					/*if(i==0 && j==0){
						mvprintw(22, 0, "(%f %f) rotated over (%d %d) produces the following image:", pos_x, pos_y, screen->size_x / 2, screen->size_y / 2);
					}*/
					pos_x -= c_x;
					pos_y -= c_y;
					pos2_x = pos_x * cos(theta) - pos_y * sin(theta);
					pos2_y = pos_x * sin(theta) + pos_y * cos(theta);
				}else{
					pos2_x = pos_x;
					pos2_y = pos_y;
				}
				/*if(pos_x < l_x || pos_x > r_x || pos_y < t_y || pos_y > b_y) {
					continue;
				}*/
				p = sprite->image->arr[i * sprite->image->size_x + j];
				if(p.c == ' ') continue;
				int cid = get_color(p);
				attron(COLOR_PAIR(cid));
				mvaddch(pos2_y, pos2_x*2, p.c); //this will lowkey print everything with an offset but that's tomorrow me's issue
				mvaddch(pos2_y, pos2_x*2+1, p.c);
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
