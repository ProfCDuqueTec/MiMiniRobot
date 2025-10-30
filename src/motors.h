#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
void stop_movement();
void move_forward(unsigned int speed_val);
void move_backwards(unsigned int speed_val);
void set_motors(int16_t l_motor_value, int16_t r_motor_value);
#endif
