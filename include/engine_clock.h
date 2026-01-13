#ifndef ENGINE_CLOCK_H
  #define ENGINE_CLOCK_H

  #include <stdint.h>
  #include <time.h>
  #include <unistd.h>

  #define baseTPS 60
  #define usleepError 500
  #define micros_per_second 1e6

  void EngineClock_init();
  void setTPS(int newTPS);
  uint64_t getTime();

  double EngineClock_getTimeElapsed();
  double EngineClock_waitForNextFrame();

#endif