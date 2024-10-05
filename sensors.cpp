#include "hardware.h"

// Measurements taken for this panel (from peak detector):
// Full sun = 3V
// Shaded, open sky, next to house wall = 1.75V
// Indoors, door shut = 1.5V
// Indoors, shaded from window = 1V

// All thresholds in approx 10mV steps

static int battery = 0;
static int panel = 0;

static const int dusk_threshold = 100, day_threshold = 150;
bool dusk() { return (panel < dusk_threshold); }
bool day() { return (panel > day_threshold); }

static const int empty_threshold = 300, full_threshold = 470, charge_threshold = 390;
bool battery_empty() { return (battery < empty_threshold); }
bool battery_full() { return (battery > full_threshold); }
bool battery_chargeable() { return (battery < charge_threshold); }

void readSensors() {
  // 1024 = 10.24V
  battery = analogRead(BATTERY);
  panel = analogRead(PANEL);
}

void dumpSensors() {
  char buf[5];
  
  Serial.begin(9600);
  Serial.print("Sensors");

  itoa(battery, buf, 10);
  Serial.print(" battery=");
  Serial.print(buf);

  itoa(panel, buf, 10);
  Serial.print(" panel=");
  Serial.print(buf);
  Serial.print("\n");
  Serial.flush();
  Serial.end();
}
