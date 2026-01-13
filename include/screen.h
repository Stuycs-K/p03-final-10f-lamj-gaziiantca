#ifndef ASCIISCREEN_H
#define ASCIISCREEN_H
#include "types.h"

hdScreen* initScreen(); //Initiates an ncurses screen
void drawSprite(const hdScreen* screen, const hdSprite* sprite); //Draws an individual sprite, this gets called by draw()
void draw(hdScreen* screen); //Draws every sprite on the screen
void addSprite(hdScreen* screen, const hdSprite* sprite); //Adds a sprite to the screen
hdSprite* initSprite(hdRawImage* img, void* extra); //Creates a sprite using an image, has an optional argument for extra content that you can do whatever you want with
void cleanupScreen(hdScreen* screen); //free()s the screen. not the sprites notably
void nukeSprite(hdSprite* sprite); //free()s a sprite
void nukeScreen(hdScreen* screen); //free()s the screen and the sprites 

#endif
