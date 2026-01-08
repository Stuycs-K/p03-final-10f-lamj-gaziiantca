#include <vector2.h>

#ifndef PLAYER_H
  #define PLAYER_H

  #define NameMaxLength 63
  #define MAX_SPEED 1

  typedef struct {
    int id;
    char name[NameMaxLength + 1];
    Vector2 pos, vel;
  } Player;

  Player* Player_new(char* name);
  void Player_updateMovement(Player* p, double dt);
  void Player_handleInput(Player* p, char c);

#endif
