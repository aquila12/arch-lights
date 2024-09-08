#include "avr/wdt.h"
#include "hardware.h"
#include "modules.h"

typedef enum { MODE_DAY, MODE_DUSK, MODE_LIGHTS, MODE_NIGHT } time_of_day;

static const int wd_timeout = WDTO_8S;
static const int tick_rate = 8;

static uint8_t seconds = 0;
static uint16_t minutes = 0;

ISR(WDT_vect) {
  seconds += tick_rate;
  while(seconds >= 60) { minutes += 1; seconds -= 60; }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(DEBUG, INPUT_PULLUP);

  initTwinkle();

  analogReference(INTERNAL1V1);

  setupWDT();

  flash(5);
}

void loop() {
  static time_of_day mode = MODE_DAY;
  time_of_day last_mode = mode;

  readSensors();

  if(digitalRead(DEBUG) == 0) dumpSensors();

  int last = seconds;
  if(mode == MODE_LIGHTS) {
    while(seconds == last) { doTwinkle(); }
  } else {
    stopTwinkle();
    enterSleep();
  }

  // FSM
  switch(mode) {
    case MODE_DAY:
    if(dusk()) mode = MODE_DUSK;
    break;

    case MODE_DUSK:
    if(day()) mode = MODE_DAY;
    if(minutes > 5) mode = MODE_LIGHTS;
    break;

    case MODE_LIGHTS:
    if(battery_empty()) mode = MODE_NIGHT;
    if(minutes > 120) mode = MODE_NIGHT;
    break;

    case MODE_NIGHT:
    if(minutes > 240 && day()) mode = MODE_DAY;
    break;
  }

  if(last_mode != mode) {
    flash(2 + mode);
    minutes = seconds = 0;
  } else if(mode != MODE_LIGHTS) flash(1);
}

static void setupWDT() {
  int t = wd_timeout;
  int prescaler = ((t & 0x8) ? bit(WDP3) : 0) + ((t & 0x4) ? bit(WDP2) : 0) +
                  ((t & 0x2) ? bit(WDP1) : 0) + ((t & 0x1) ? bit(WDP0) : 0);

  int setting = prescaler | bit(WDIE); // Interrupt mode only, no reset

  cli();
  wdt_reset();
  MCUSR &= ~bit(WDRF); // Clear watchdog reset (it overrides WDE)
  _WD_CONTROL_REG |= bit(WDCE) | bit(WDE); // Enable changes
  _WD_CONTROL_REG = setting;
  sei();
}

static void enterSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  power_all_disable();
  sleep_mode();
  // Continues after WDT
  power_all_enable();
}

void doClock() {
  static unsigned int last = seconds;
  if(last == seconds) return;
  last = seconds;

  if(seconds > 0) flash((seconds % 10) ? 1 : 2);
  else { flash(3); }
}

void flash(int n) {
  while(n--) {
    digitalWrite(LED, 1);
    delay(10);
    digitalWrite(LED, 0);
    if(n) delay(240);
  }
}
