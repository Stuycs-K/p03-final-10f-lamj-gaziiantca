#include "player.h"

#include <ncurses.h>
#include <stdlib.h>

#include <string.h>


const double konstant = 1.0f/2.0f;

void Player_init(Player* self, char* name) {
  strncpy(self->name, name, NameMaxLength);
  self->pos = Vector2_new(0, 0);
  self->vel = Vector2_new(0, 0);

  self->moved = Signal_new();

  self->id = 1; // Change this later
}

typedef struct {
  Vector2 lastPos;
  Vector2 newPos;
} PlayerMovedEvent;

void Player_updateMovement(Player* self, double dt) {
  Vector2 oldPos = self->pos;
  self->pos = Vector2_add(self->pos, Vector2_scale(self->vel, dt * MAX_SPEED));
  PlayerMovedEvent event = {oldPos, self->pos};

  Signal_Fire(self->moved, &event);
}

typedef struct {
  Player* player;
  int* collisionMap;
  int cmapWidth, cmapHeight;
} CollisionCtx;

void returnToLastValidPos(void* context, void* args) {
  CollisionCtx* con = (CollisionCtx*) context;
  PlayerMovedEvent* event = (PlayerMovedEvent*) args;
    mvprintw(13, 0, "AAAAAAAAAAAAAAAAAAAAAAAAA");
  
  int xPos = event->newPos.x;
  int yPos = event->newPos.y;

  int* cmap = con->collisionMap;
  int cmapWidth = con->cmapWidth;
  int cmapHeight = con->cmapHeight;

  int playerLocationIndex = yPos * cmapHeight + xPos;
  if (playerLocationIndex < 0 || cmapWidth * cmapHeight <= playerLocationIndex) {
    return;
  }

  if (1 == cmap[playerLocationIndex]) {
    // 1 indicates player is on a collidable
    con->player->pos = event->lastPos;
  } else {
    // Do nothing
  }
}

void Player_enableCollision(Player* self, hdScreen* screen) {
  CollisionCtx* context = calloc(1, sizeof(CollisionCtx));
  context->player = self;
  context->collisionMap = screen->collisionMap;
  context->cmapWidth = screen->cmap_width;
  context->cmapHeight = screen->cmap_height;
  Signal_Connect(self->moved, &returnToLastValidPos, context);
}

void Player_handleInput(Player* self, char c) {
  switch(c) {
    case '\0': self->vel = Vector2_new(0,0); break;
    case 'w': case 'W': self->vel = Vector2_new(0,1); break;
    case 'a': case 'A': self->vel = Vector2_new(-1,0); break;
    case 's': case 'S': self->vel = Vector2_new(0,-1); break;
    case 'd': case 'D': self->vel = Vector2_new(1,0); break;
	default: self->vel = Vector2_new(0,0); break;
  }
}
