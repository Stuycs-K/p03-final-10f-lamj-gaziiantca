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

void testEvent(void* context, void* args) { // Check if player x pos is >= 10
  int* editedVar = (int*) context;
  Vector2 pos = *((Vector2*) args);

  if (pos.x >= 10) {
    *editedVar = 1; 
  }
}


void testBudgetGameLoop() {
  EngineClock_init();

  Player playerStruct;
  Player* newPlayer = &playerStruct;
  Player_init(newPlayer, "Aleksandr");

  int editedVar = 0;
  Signal_Connect(newPlayer->moved, &testEvent, &editedVar);

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

    if (editedVar) {
      mvprintw(4, 0, "Player x position has passed 10!!");
    }

    refresh();
  }

  exitGame(0);
}

int main(){
	// loadRawImage("assets/testsprite.txt");
  testBudgetGameLoop();
}