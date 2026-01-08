#include <player.h>

#include <string.h>
#include <stdlib.h>

Player* Player_new(char* name) {
  Player* newPlayer = (Player*) malloc(sizeof(Player));
  strncpy(newPlayer->name, name, NameMaxLength);
  newPlayer->pos = Vector2_new(0, 0);
  newPlayer->vel = Vector2_new(0, 0);

  newPlayer->id = 1; // Change this later

  return newPlayer;
}

void Player_updateMovement(Player* p, double dt) {
  p->pos = Vector2_add(p->pos, Vector2_scale(p->vel, dt));
}

void Player_handleInput(Player *p, char c) {
  switch(c) {
    case '\0': p->vel = Vector2_new(0,0); break;
    case 'w': case 'W': p->vel = Vector2_new(0,1); break;
    case 'a': case 'A': p->vel = Vector2_new(-1,0); break;
    case 's': case 'S': p->vel = Vector2_new(0,-1); break;
    case 'd': case 'D': p->vel = Vector2_new(1,0); break;
  }
}
