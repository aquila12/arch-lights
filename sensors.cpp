#include "hardware.h"

// Measurements taken for this panel (from peak detector):
// Full sun = 3V
// Shaded, open sky, next to house wall = 1.75V
// Indoors, door shut = 1.5V
// Indoors, shaded from window = 1V

// All in approx 10mV steps
static const int battery_threshold = 300, dusk_threshold = 100, day_threshold = 150;

static int battery = 0;
static int panel = 0;

bool dusk() { return (panel < dusk_threshold); }
bool day() { return (panel > day_threshold); }
bool battery_empty() { return (battery < battery_threshold); }

void readSensors() {
  // 1024 = 10.24V
  battery = analogRead(BATTERY);
  panel = analogRead(PANEL);
}

void dumpSensors() {
  Serial.begin(9600);
  Serial.print("Sensors");
  Serial.print(" battery=");
  Serial.print(battery);
  Serial.print(" panel=");
  Serial.print(panel);
  Serial.println();
  Serial.flush();
  Serial.end();
}
