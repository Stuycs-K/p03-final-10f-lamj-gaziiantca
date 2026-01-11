#ifndef ASCIISCREEN_H
#define ASCIISCREEN_H
#include "types.h"

hdScreen* initScreen();
void drawSprite(const hdScreen* screen, const hdSprite* sprite);
void draw(hdScreen* screen);
void addSprite(hdScreen* screen, const hdSprite* sprite);
hdSprite* initSprite(hdRawImage* img, void* extra);
void cleanupScreen(hdScreen* screen);
void nukeSprite(hdSprite* sprite);
void nukeScreen(hdScreen* screen);

#endif
