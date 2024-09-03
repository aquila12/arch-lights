#include "hardware.h"

void initTwinkle() {
  pinMode(STRING_AL, OUTPUT);
  pinMode(STRING_BL, OUTPUT);
  pinMode(STRING_YL, OUTPUT);
  digitalWrite(STRING_AL, 0);
  digitalWrite(STRING_BL, 0);
  digitalWrite(STRING_YL, 0);

  pinMode(STRING_AH, OUTPUT);
  pinMode(STRING_BH, OUTPUT);
  pinMode(STRING_YH, OUTPUT);
  digitalWrite(STRING_AH, 1);
  digitalWrite(STRING_BH, 1);
  digitalWrite(STRING_YH, 1);
}

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
  int off = set ? 0 : 1;
  int on = set ? 1 : 0;

  int a_pin = set ? STRING_AL : STRING_AH;
  int b_pin = set ? STRING_BL : STRING_BH;
  int y_pin = set ? STRING_YH : STRING_YL;

  int a = intensity(step[set]);
  int b = intensity(step[set + 2]);

  // Turn on high or low side common (NB reverse polarity)
  digitalWrite(y_pin, 1 - on);

  // Bitbang PWM
  for(int i=100; i>0; --i) {
    digitalWrite(a_pin, (--a > 0) ? on : off);
    digitalWrite(b_pin, (--b > 0) ? on : off);
    delayMicroseconds(20);
  }

  // Turn off high or low side common (NB reverse polarity)
  digitalWrite(y_pin, 1 - off);
  digitalWrite(a_pin, off);
  digitalWrite(b_pin, off);

  // Swap sets next time
  set = 1 - set;
}

void stopTwinkle() {
  digitalWrite(STRING_AL, 0);
  digitalWrite(STRING_BL, 0);
  digitalWrite(STRING_YL, 0);

  digitalWrite(STRING_AH, 1);
  digitalWrite(STRING_BH, 1);
  digitalWrite(STRING_YH, 1);
}
