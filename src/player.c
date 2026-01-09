#include <player.h>

#include <string.h>
#include <stdlib.h>

void Player_init(Player* self, char* name) {
  strncpy(self->name, name, NameMaxLength);
  self->pos = Vector2_new(0, 0);
  self->vel = Vector2_new(0, 0);

  self->id = 1; // Change this later
}

void Player_updateMovement(Player* self, double dt) {
  self->pos = Vector2_add(self->pos, Vector2_scale(self->vel, dt));
}

void Player_handleInput(Player* self, char c) {
  switch(c) {
    case '\0': self->vel = Vector2_new(0,0); break;
    case 'w': case 'W': self->vel = Vector2_new(0,1); break;
    case 'a': case 'A': self->vel = Vector2_new(-1,0); break;
    case 's': case 'S': self->vel = Vector2_new(0,-1); break;
    case 'd': case 'D': self->vel = Vector2_new(1,0); break;
  }
}
