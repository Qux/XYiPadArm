/*
 * Rotation....
   Stop at: 93
   Speed: +- 30?
*/

#include <Servo.h>

constexpr char ROTATION_SERVO_PIN = 10;
constexpr char ROTATION_SERVO_CONTROL_POT_PIN = A0;

constexpr char NORMAL_SERVO_PIN = 9;
constexpr char NORMAL_SERVO_CONTROL_POT_PIN = A1;

Servo rotation_servo;
Servo normal_servo;

int pot_val, angle;

void setup() {
  // put your setup code here, to run once:
  rotation_servo.attach(ROTATION_SERVO_PIN);
  normal_servo.attach(NORMAL_SERVO_PIN);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  pot_val = analogRead(ROTATION_SERVO_CONTROL_POT_PIN);
  angle = map(pot_val, 0, 1023, 60, 120);
  rotation_servo.write(angle);

  pot_val = analogRead(NORMAL_SERVO_CONTROL_POT_PIN);
  angle = map(pot_val, 0, 1023, 0, 179);
  normal_servo.write(angle);
}
