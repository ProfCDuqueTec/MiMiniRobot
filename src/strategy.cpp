#include "strategy.h"
#include "motors.h"
#include "sensors.h"

#define PIN_RED_LED 13
#define PIN_DIP_SW_1 7

// Parámetros base
static const uint16_t POWER_HIGH = 230;
static const uint16_t POWER_MED  = 140;
static const uint16_t POWER_LOW  = 90;
static const uint16_t BURST_DURATION_MS = 300;

void strategy_init() {
  pinMode(PIN_DIP_SW_1, INPUT_PULLUP);
}

bool kata_burst_frontal() {
  read_all_sensors();
  if (line_sensor_left || line_sensor_right) return false;
  unsigned long t0 = millis();
  while (millis() - t0 < BURST_DURATION_MS) {
    set_motors(POWER_HIGH, POWER_HIGH);
    read_all_sensors();
    if (line_sensor_left || line_sensor_right) { stop_movement(); return false; }
  }
  stop_movement();
  return true;
}

// ... otras funciones stub omitidas para brevedad ...

void execute_strategy() {
  read_all_sensors();
  if (sensor_middle) {
    kata_burst_frontal();
  } else {
    set_motors(120, 120);
  }
}
