#include "avr/wdt.h"
#include "EEPROM.h"
#include "hardware.h"
#include "modules.h"

typedef enum { MODE_DAY, MODE_DUSK, MODE_LIGHTS, MODE_NIGHT } time_of_day;

static const int wd_timeout = WDTO_8S;
static const int tick_rate = 8;

static uint8_t seconds = 0;
static uint16_t minutes = 0;
static uint16_t mode_mins = 0;
static uint16_t night_mins;
static int16_t light_mins = 0;

ISR(WDT_vect) {
  seconds += tick_rate;
  while(seconds >= 60) { minutes += 1; seconds -= 60; }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(CHARGE, INPUT);
  pinMode(DUMP_SENSORS, INPUT_PULLUP);
  pinMode(BOOT_MODE, INPUT_PULLUP);

  initTwinkle();

  analogReference(INTERNAL1V1);

  setupWDT();

  EEPROM.get(0, night_mins);
  if(night_mins < 1 * 60 || night_mins > 23 * 60) night_mins = 12 * 60; // Default if data looks invalid

  flash(5);
}

void loop() {
  static time_of_day mode = digitalRead(BOOT_MODE) ? MODE_DAY : MODE_LIGHTS;
  time_of_day last_mode = mode;

  readSensors();

  if(digitalRead(DUMP_SENSORS) == 0) dumpSensors();

  // Charge handling - pull to ground
  if(battery_full()) {
    pinMode(CHARGE, OUTPUT);
    digitalWrite(CHARGE, 0);
  } else if(battery_chargeable()) {
    pinMode(CHARGE, INPUT);
  }

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
    if(mode_mins > 5) mode = MODE_LIGHTS;
    break;

    case MODE_LIGHTS:
    if(battery_empty()) mode = MODE_NIGHT;
    if(mode_mins > light_mins) mode = MODE_NIGHT;
    break;

    case MODE_NIGHT:
    if(mode_mins > 240 && day()) mode = MODE_DAY;
    break;
  }

  if(last_mode != mode) {
    flash(2 + mode);
    mode_mins = 0;

    switch(mode) {
      case MODE_DAY:
      night_mins = minutes;
      EEPROM.update(0, night_mins);
      minutes = 0;
      break;

      case MODE_DUSK:
      if(night_mins < 480) light_mins = 120;  // Min 2h
      else light_mins = night_mins / 2 - 120; // About 2h before solar midnight
      minutes = 0;
      break;
    }
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
  ADCSRA = 0;
  power_all_disable();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
  // Continues after WDT
  power_all_enable();
  ADCSRA = bit(ADEN);
}

void flash(int n) {
  while(n--) {
    digitalWrite(LED, 1);
    delay(10);
    digitalWrite(LED, 0);
    if(n) delay(240);
  }
}
