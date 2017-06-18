#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>
#include <PowerFunctions.h>

MeIR ir;
PowerFunctions pf(3, 0);

void setup() {
}

void loop() {
  lego(RED, PWM_REV1, 500);
  delay(200);
}

void lego(uint8_t output, uint8_t pwm,  uint16_t time) {
  pf.single_pwm(output, pwm);
  delay(time);
  pf.single_pwm(output, PWM_BRK);
  delay(30);
  pf.single_pwm(output, PWM_FLT);
}
