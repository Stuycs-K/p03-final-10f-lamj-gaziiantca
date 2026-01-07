#include <player.h>

Player* Player_new(char* name) {
  Player* newPlayer = (Player*) malloc(sizeof(Player));
  strncpy(newPlayer.name, name, NameMaxLength);
  newPlayer.pos = Vec2_new(0, 0);
  newPlayer.vel = Vec2_new(0, 0);

  return newPlayer;
}

void Player_updateMovement(Player p, double dt) {
  p.pos = Vec2_add(p.pos, Vec2_scale(p.vel, dt));
}
