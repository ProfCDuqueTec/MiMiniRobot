#include "motors.h"
#define PIN_MOTOR_LEFT_1 6
#define PIN_MOTOR_LEFT_2 5
#define PIN_MOTOR_RIGHT_1 11
#define PIN_MOTOR_RIGHT_2 10
void stop_movement() {
  digitalWrite(PIN_MOTOR_LEFT_1, HIGH);
  digitalWrite(PIN_MOTOR_LEFT_2, HIGH);
  digitalWrite(PIN_MOTOR_RIGHT_1, HIGH);
  digitalWrite(PIN_MOTOR_RIGHT_2, HIGH);
}
void move_forward(unsigned int s) {
  if (s > 255) s = 255;
  analogWrite(PIN_MOTOR_LEFT_1, s);
  digitalWrite(PIN_MOTOR_LEFT_2, LOW);
  analogWrite(PIN_MOTOR_RIGHT_1, s);
  digitalWrite(PIN_MOTOR_RIGHT_2, LOW);
}
void move_backwards(unsigned int s) {
  if (s > 255) s = 255;
  analogWrite(PIN_MOTOR_LEFT_2, s);
  digitalWrite(PIN_MOTOR_LEFT_1, LOW);
  analogWrite(PIN_MOTOR_RIGHT_2, s);
  digitalWrite(PIN_MOTOR_RIGHT_1, LOW);
}
void set_motors(int16_t l, int16_t r) {
  if (l > 0) { digitalWrite(PIN_MOTOR_LEFT_1, HIGH); analogWrite(PIN_MOTOR_LEFT_2, 255 - l); }
  else { digitalWrite(PIN_MOTOR_LEFT_2, HIGH); analogWrite(PIN_MOTOR_LEFT_1, 255 + l); }
  if (r > 0) { digitalWrite(PIN_MOTOR_RIGHT_1, HIGH); analogWrite(PIN_MOTOR_RIGHT_2, 255 - r); }
  else { digitalWrite(PIN_MOTOR_RIGHT_2, HIGH); analogWrite(PIN_MOTOR_RIGHT_1, 255 + r); }
}
