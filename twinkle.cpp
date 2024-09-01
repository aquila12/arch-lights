#include "hardware.h"

inline static int intensity(int phase) {
  if(phase < 100) return phase;
  if(phase < 120) return 100;
  if(phase < 220) return 220 - phase;
  return 0;
}

void doTwinkle() {
  static unsigned long tock[] = {0, 500, 1000, 1500};
  static const unsigned step_time[] = { 43, 53, 61, 71 }; // Some primes
  static int step[] = {50, 100, 150, 200};
  static const int rate = 3;
  static const int steps = 250;

  // Update states
  unsigned long t = millis();
  for(int i=0; i<4; ++i) {
    if(t < tock[i]) continue;

    tock[i] += step_time[i];
    step[i] += rate;
    if(step[i] >= steps) step[i] -= steps;
  }

  // Iterate between each set
  static int set = 0;
  int off = set ? 1 : 0;
  int on = set ? 0 : 1;

  int a = intensity(step[set]);
  int b = intensity(step[set + 2]);

  // Bitbang PWM
  digitalWrite(STRING_Y, set);
  for(int i=100; i>0; --i) {
    digitalWrite(STRING_A, (--a > 0) ? on : off);
    digitalWrite(STRING_B, (--b > 0) ? on : off);
    delayMicroseconds(20);
  }

  set = 1 - set;
}

void stopTwinkle() {
  digitalWrite(STRING_Y, 0);
  digitalWrite(STRING_A, 0);
  digitalWrite(STRING_B, 0);
}