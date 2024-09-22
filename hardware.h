#include "Arduino.h"

// ATMega8/48/88/168/328
static const int
/* !RESET */  pin1 = 22,  pin28 = 19, // x
/*    Rxd */  pin2 = 0,   pin27 = 18, // x
/*    Txd */  pin3 = 1,   pin26 = 17, // x
/*    LED */  pin4 = 2,   pin25 = 16, // PANEL
/*      x */  pin5 = 3,   pin24 = 15, // BATTERY
/* CHARGE */  pin6 = 4,   pin23 = 14, // x
              /* Vcc */   /* GND */
              /* GND */   /* Aref */
/*     YH */  pin9 = 20,  /* AVcc */
/*     YL */  pin10 = 21, pin19 = 13, // SCK
/*     AH */  pin11 = 5,  pin18 = 12, // MISO
/*     AL */  pin12 = 6,  pin17 = 11, // MOSI
/*     BH */  pin13 = 7,  pin16 = 10, // BOOT_MODE
/*     BL */  pin14 = 8,  pin15 = 9;  // DUMP_SENSORS

#define LED pin4
#define CHARGE pin6

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