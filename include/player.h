#include <vector2.h>

#ifndef PLAYER_H
  #define PLAYER_H

  #define NameMaxLength 63

  typedef struct {
    char name[NameMaxLength + 1];
    Vec2 pos, vel;
  } Player;

  Player* Player_new(char* name);

  void Player_updateMovement(Player p, double dt);

#endif
