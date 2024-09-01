#include "Arduino.h"

// ATMega8/48/88/168/328
static const int
 /* !RESET */ pin1 = 22,  pin28 = 19,
    /* Rxd */ pin2 = 0,   pin27 = 18,
    /* Txd */ pin3 = 1,   pin26 = 17,
              pin4 = 2,   pin25 = 16,
              pin5 = 3,   pin24 = 15,
              pin6 = 4,   pin23 = 14,
              /* Vcc */   /* GND */
              /* GND */   /* Aref */
              pin9 = 20,  /* AVcc */
              pin10 = 21, pin19 = 13, // SCK
              pin11 = 5,  pin18 = 12, // MISO
              pin12 = 6,  pin17 = 11, // MOSI
              pin13 = 7,  pin16 = 10,
              pin14 = 8,  pin15 = 9;

#define LED pin4
#define STRING_A pin15
#define STRING_B pin16
#define STRING_Y pin13
#define BATTERY pin23
#define PANEL pin24