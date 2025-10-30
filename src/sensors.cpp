#include "sensors.h"
int L_LINE_REF_VALUE = 200; int R_LINE_REF_VALUE = 200;
bool sensor_left = false, sensor_middle = false, sensor_right = false;
bool line_sensor_left = false, line_sensor_right = false;
void read_all_sensors() {
  sensor_left = digitalRead(A4);
  sensor_middle = digitalRead(A3);
  sensor_right = digitalRead(A2);
  line_sensor_left = (analogRead(A5) < L_LINE_REF_VALUE);
  line_sensor_right = (analogRead(A1) < R_LINE_REF_VALUE);
}
void calibrate_line_sensors() {
  int sL = 0, sR = 0; 
  for (int i = 0; i < 50; i++) {
    sL += analogRead(A5);
    sR += analogRead(A1);
    delay(10);
  }
  L_LINE_REF_VALUE = (sL / 50) - 50;
  R_LINE_REF_VALUE = (sR / 50) - 50;
}
