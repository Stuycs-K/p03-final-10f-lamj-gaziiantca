#include "engine_clock.h"
#include <stdint.h>

static int TPS;
static uint64_t startTime;
static uint64_t latestFrameTime;
static uint64_t deltaTime;

void EngineClock_init() {
  TPS = baseTPS;
  startTime = getTime();
  latestFrameTime = startTime;
}

void setTPS(int newTPS) {
  TPS = newTPS;
}

uint64_t getTime() { //returns microseconds
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  
  return ((uint64_t)ts.tv_sec * micros_per_second) + ((uint64_t)ts.tv_nsec / 1e3);
}

double timeElapsed() {
  return ((double) latestFrameTime - startTime) / micros_per_second;
}

