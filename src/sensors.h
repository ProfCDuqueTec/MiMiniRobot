#ifndef SENSORS_H
#define SENSORS_H
#include <Arduino.h>
extern int L_LINE_REF_VALUE;
extern int R_LINE_REF_VALUE;
extern bool sensor_left, sensor_middle, sensor_right;
extern bool line_sensor_left, line_sensor_right;
void read_all_sensors();
void calibrate_line_sensors();
#endif
