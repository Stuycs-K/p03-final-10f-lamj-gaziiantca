#include <stdlib.h>
#include <ncurses.h>

#include "engine_clock.h"
#include "player.h"
#include "ascii_image.h"
#include "event_signals.h"
#include "vector2.h"

void exitGame(int n){
	curs_set(1);
	endwin();
	exit(n);
}

int get_wasd_input() {
  int ch = getch();
  if (
    ch == 'w' || ch == 'W' || 
    ch == 'a' || ch == 'A' || 
    ch == 's' || ch == 'S' || 
    ch == 'd' || ch == 'D') {
      return ch;
  }

  return 0;
}

typedef struct { 
  int var; 
  Vector2 lastPos;
} MovedCtx;

void testEvent(void* context, void* args) { // Check if player x pos is >= 10
  MovedCtx* con = (MovedCtx*) context;
  Vector2 pos = *((Vector2*) args);

  Vector2 delta = Vector2_sub(pos, con->lastPos);
  con->lastPos = pos;

  if (Vector2_mag(delta) > 0) {
    mvprintw(4, 0, "Position changed by (%.3lf, %.3lf)", delta.x, delta.y);
  }

  if (pos.x >= 10) {
    con->var = 1; 
  }
}

void testBudgetGameLoop() {
  EngineClock_init();

  Player playerStruct;
  Player* newPlayer = &playerStruct;
  Player_init(newPlayer, "Aleksandr");

  MovedCtx* context = calloc(1, sizeof(MovedCtx));
  Signal_Connect(newPlayer->moved, &testEvent, context);

  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  curs_set(0);

  while (1) {
    clear();
    double dt = EngineClock_waitForNextFrame();
    char input = get_wasd_input();
    Player_handleInput(newPlayer, input);
    Player_updateMovement(newPlayer, dt);

    mvprintw(0, 0, "Time Elapsed: %.2lfs", EngineClock_getTimeElapsed());
    mvprintw(1, 0, "Pos: (%.2lf, %.2lf)", newPlayer->pos.x, newPlayer->pos.y);
    mvprintw(2, 0, "dt=%lf | TPS=%.2f\n", dt, 1/dt);
    mvprintw(3, 0, "Current input: %c", input);

    if (context->var) {
      mvprintw(5, 0, "Player x position has passed 10!!");
    }

    refresh();
  }

  exitGame(0);
}

int main(){
	// loadRawImage("assets/testsprite.txt");
  testBudgetGameLoop();
}