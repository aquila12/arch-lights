#include "hardware.h"

static const int battery_threshold = 300, dusk_threshold = 180, day_threshold = 220;

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