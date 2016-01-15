#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>

Servo servo;
MeIR ir;
MeLEDMatrix matrix(1);
MePort port(3);
MeUltrasonicSensor ultrasonic(4);
MeDCMotor motor_left(M1);
MeDCMotor motor_right(M2);
MeRGBLed led_rgb(7, 7==7?2:4);
MeBuzzer buzzer;

int state;
int move_speed;
double angle_front;
double angle_right;
double angle_left;
double distance;
double distance_sweep;
double distance_warn;

enum
{
  MODE_A,
  MODE_B,
  MODE_C
};

uint8_t mode = MODE_A;

void setup() {

  state = 0;
  move_speed = 150;
  angle_right = 50;
  angle_front = 90;
  angle_left = 130;
  distance_sweep = 35;
  distance_warn = 15;

  buzzer.tone(393, 200);
  matrix.clearScreen();
  led_rgb.setColor(0,0,100,0);
  led_rgb.show();
  delay(100);

  servo.attach(port.pin1());
  servo.write(angle_front);

  //ir.begin();

}

void loop()
{
  while(1)
  {
    get_ir_command();
    //serialHandle();
    switch (mode)
    {
    case MODE_A:
      mode_stop();
      led_rgb.setColor(0,0,0,0);
      led_rgb.show();
      break;
    case MODE_B:
      mode_avoid();
      break;
    case MODE_C:
      break;
    }
  }
}

void mode_stop() {
  move_not();
  servo.write(angle_front);
  delay(100);
}

void mode_avoid() {

  randomSeed(analogRead(6));
  uint8_t randNumber = random(2);

  distance = get_distance();

  if (distance > distance_sweep || distance == 0) {
    led_rgb.setColor(0,0,100,0);
    led_rgb.show();
    move_forward();
  }

  else if (distance > distance_warn) {
    move_not();
    move_choice();
  }

  else {
    led_rgb.setColor(0,100,0,0);
    led_rgb.show();
    move_backward();
    delay(2000);
    switch (randNumber) {
    case 0:
      move_left();
      delay(600);
      break;
    case 1:
      move_right();
      delay(600);
      break;
    }
  }
  delay(200);
}


void move_choice() {
  int distance_right, distance_left;

  servo.write(angle_right);
  distance_right = get_distance();
  delay(300);

  servo.write(angle_left);
  distance_left = get_distance();
  delay(300);

  servo.write(angle_front);

  if (distance_right <= distance_left) {
    led_rgb.setColor(2,0,0,100);
    led_rgb.show();
    move_left();
    delay(300);
  } else {
    led_rgb.setColor(1,0,0,100);
    led_rgb.show();
    move_right();
    delay(300);
  }
}

int get_distance_mean() {
  int i, distance;

  for (i=0; i<5; i++) {
    distance += ultrasonic.distanceCm();
  }
  return distance /= 5;
}

int get_distance() {
  int i, distance;
  distance = ultrasonic.distanceCm();
  return distance;
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

void get_ir_command()
{
  static long time = millis();
  if (ir.decode())
  {
    uint32_t value = ir.value;
    time = millis();
    switch (value >> 16 & 0xff)
    {
    case IR_BUTTON_A:
      mode = MODE_A;
      buzzer.tone(393, 200);
      break;

    case IR_BUTTON_B:
      mode = MODE_B;
      buzzer.tone(393, 200);
      break;
    }
  }
  else if (millis() - time > 120)
  {
    time = millis();
  }
}
