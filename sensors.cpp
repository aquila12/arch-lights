#include "hardware.h"

static const int battery_threshold = 200, dusk_threshold = 180, day_threshold = 220;

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