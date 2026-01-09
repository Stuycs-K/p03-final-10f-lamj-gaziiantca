#ifndef PLAYER_H
  #define PLAYER_H

  #include <vector2.h>

  #define NameMaxLength 63
  #define MAX_SPEED 1

  typedef struct Player {
    int id;
    char name[NameMaxLength + 1];
    Vector2 pos, vel;
  } Player;

  void Player_init(Player* self, char* name);
  void Player_updateMovement(Player* self, double dt);
  void Player_handleInput(Player* self, char c);

#endif
