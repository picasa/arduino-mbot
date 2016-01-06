#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>

double state;
double move_speed;
double angle_front;
double angle_right;
double angle_left;
double distance;
double distance_sweep;
double distance_warn;

Servo servo;
MeUltrasonicSensor ultrasonic(3);
MePort port(4);
MeDCMotor motor_left(M1);
MeDCMotor motor_right(M2);
MeRGBLed led_rgb(7, 7==7?2:4);
MeBuzzer buzzer;

void setup() {

  state = 0;
  move_speed = 200;
  angle_right = 50;
  angle_front = 90;
  angle_left = 130;
  distance_sweep = 40;
  distance_warn = 15;

  buzzer.tone(393, 200);

  servo.attach(port.pin1());
  servo.write(angle_front);
}

void loop() {

  check_button();

  if (state==0) {
    move_not();
  }

  else {
    distance = get_distance();

    if (distance > distance_sweep || distance == 0) {
      led_rgb.setColor(0,0,100,0);
      led_rgb.show();
      move_forward();
    }

    else if (distance >= distance_warn) {
      led_rgb.setColor(0,100,100,100);
      led_rgb.show();
      move_not();
      move_choice();
    }

    else {
      led_rgb.setColor(0,100,0,0);
      led_rgb.show();
      move_backward();
      delay(1500);
    }
    delay(200);
  }
}

int get_distance() {
  int i, distance;

  for (i=0; i<5; i++) {
    distance += ultrasonic.distanceCm();
  }
  return distance /= 5;
}

void move_choice() {
  int distance_right, distance_left;

  servo.write(angle_right);
  delay(200);
  led_rgb.setColor(1,0,0,100);
  led_rgb.show();
  distance_right = get_distance();

  servo.write(angle_left);
  delay(200);
  led_rgb.setColor(2,0,0,100);
  led_rgb.show();
  distance_left = get_distance();

  servo.write(angle_front);

  if (distance_right <= distance_left) {
    move_left();
    delay(300);
  } else {
    move_right();
    delay(300);
  }
}

void move_forward() {
  motor_left.run(-move_speed);
  motor_right.run(move_speed);
}

void move_backward() {
  motor_left.run(move_speed);
  motor_right.run(-move_speed);
}

void move_left() {
  motor_left.run(-move_speed/10);
  motor_right.run(move_speed);
}

void move_right() {
  motor_left.run(-move_speed);
  motor_right.run(move_speed/10);
}

void move_not() {
  motor_left.run(0);
  motor_right.run(0);
}

void check_button() {
  if((0^(analogRead(A7)>10?0:1))){
    if (state > 0) {
      state = 0;
      delay(200);
    } else {
      state = 1;
      delay(200);
    }
    while(!((1^(analogRead(A7)>10?0:1)))){}
  }
}
