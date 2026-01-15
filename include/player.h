#ifndef PLAYER_H
  #define PLAYER_H

  #include "vector2.h"
  #include "event_signals.h"
  #include "screen.h"

  #define NameMaxLength 63
  #define MAX_SPEED 20

  typedef struct Player {
    int id;
    char name[NameMaxLength + 1];
    Vector2 pos, vel;

    Signal* moved;
  } Player;

  void Player_init(Player* self, char* name);
  void Player_updateMovement(Player* self, double dt);
  void Player_handleInput(Player* self, char c);

  struct CollisionContext;
  struct PlayerMovedEvent;
  void returnToValidPos(void* context, void* args);
  void Player_enableCollision(Player* self, hdScreen* screen);

#endif
