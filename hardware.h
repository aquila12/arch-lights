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
#define STRING_YH pin9
#define STRING_YL pin10
#define STRING_AH pin11
#define STRING_AL pin12
#define STRING_BH pin13
#define STRING_BL pin14

#define BATTERY pin24
#define PANEL pin25

#define DUMP_SENSORS pin15
#define BOOT_MODE pin16