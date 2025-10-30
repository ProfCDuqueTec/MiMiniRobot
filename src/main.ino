#include "sensors.h"
#include "motors.h"
#include "strategy.h"
#define PIN_SENSOR_LEFT A4
#define PIN_SENSOR_MIDDLE A3
#define PIN_SENSOR_RIGHT A2
#define PIN_LINE_SENSOR_LEFT A5
#define PIN_LINE_SENSOR_RIGHT A1
#define PIN_MOTOR_LEFT_1 6
#define PIN_MOTOR_LEFT_2 5
#define PIN_MOTOR_RIGHT_1 11
#define PIN_MOTOR_RIGHT_2 10
#define PIN_RED_LED 13
#define PIN_START_MODULE 3
#define PIN_DIP_SW_1 7
int start_module_status = 0;
void setup() {
  pinMode(PIN_SENSOR_LEFT, INPUT);
  pinMode(PIN_SENSOR_MIDDLE, INPUT);
  pinMode(PIN_SENSOR_RIGHT, INPUT);
  pinMode(PIN_LINE_SENSOR_LEFT, INPUT);
  pinMode(PIN_LINE_SENSOR_RIGHT, INPUT);
  pinMode(PIN_MOTOR_LEFT_1, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_2, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_1, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_2, OUTPUT);
  pinMode(PIN_RED_LED, OUTPUT);
  pinMode(PIN_START_MODULE, INPUT);
  pinMode(PIN_DIP_SW_1, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_START_MODULE), end_match, FALLING);
  calibrate_line_sensors();
  while (start_module_status == 0) {
    start_module_status = digitalRead(PIN_START_MODULE);
  }
}
void loop() {
  execute_strategy();
}
