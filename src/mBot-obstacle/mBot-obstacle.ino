#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>

Servo servo;
MeIR ir;
MeLEDMatrix matrix(1);
//MeLineFollower line(2);
MePort port_back(2);
MeUltrasonicSensor ultrasonic(3);
MePort port_front(4);
MeDCMotor motor_left(M1);
MeDCMotor motor_right(M2);
MeRGBLed led_rgb(7, 7==7?2:4);
MeBuzzer buzzer;

int state;
int detection_front_right;
int detection_front_left;
int move_speed;
double angle_front;
double angle_left;
double angle_right;
double distance;
double distance_sweep;
double distance_warn;
unsigned char draw_buffer[16];
unsigned char *draw_temp;

enum
{
  MODE_A,
  MODE_B
};

uint8_t mode = MODE_A;

void setup() {

  state = 0;
  move_speed = 200;
  angle_left = 50;
  angle_front = 90;
  angle_right = 130;
  distance_sweep = 35;
  distance_warn = 15;

  buzzer.tone(393, 200);
  matrix.clearScreen();
  led_rgb.setColor(0,0,100,0);
  led_rgb.show();
  delay(100);

  servo.attach(port_front.pin1());

}

void loop()
{
  while(1)
  {
    check_button();

    switch (mode)
    {
    case MODE_A:
      mode_stop();
      break;
    case MODE_B:
      mode_avoid();
      break;
    }
  }
}

void mode_stop() {
  move_not();
  servo.write(angle_front);
  led_rgb.setColor(0,0,0,0);
  led_rgb.show();
  //draw_stop();
  delay(100);
}

void mode_avoid() {

  distance = get_distance();
  detection_front_right = port_back.dpRead1();
  detection_front_left = port_back.dpRead2();

  if (distance > distance_sweep || distance == 0) {
    matrix.clearScreen();
    //draw_arrow_straight();

    move_forward();

    if (detection_front_left == 0 || detection_front_right == 0) {
      matrix.clearScreen();
      draw_warning();

      move_backward();
      delay(2000);
    }
  }

  else if (distance > distance_warn) {
    matrix.clearScreen();
    draw_question();

    move_not();
    move_choice();
  }

  else {
    draw_warning();

    move_backward();
    delay(2000);
    move_random();

  }
  delay(200);
}

// choose direction to move if distance < distance_sweep
void move_choice() {
  int distance_left, distance_right;

  // get distance in two directions
  servo.write(angle_left);
  distance_left = get_distance();
  delay(300);

  servo.write(angle_right);
  distance_right = get_distance();
  delay(300);

  servo.write(angle_front);

  // move the opposite direction of shortest distance
  if (distance_left <= distance_right) {
    move_right();
    delay(300);
  } else {
    move_left();
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
  led_rgb.setColor(0,0,100,0);
  led_rgb.show();

  motor_left.run(-move_speed);
  motor_right.run(move_speed);
}

void move_backward() {
  led_rgb.setColor(0,100,0,0);
  led_rgb.show();

  motor_left.run(move_speed);
  motor_right.run(-move_speed);
}

void move_right() {
  led_rgb.setColor(1,0,0,100);
  led_rgb.show();

  motor_left.run(-move_speed/10);
  motor_right.run(move_speed);
}

void move_left() {
  led_rgb.setColor(2,0,0,100);
  led_rgb.show();

  motor_left.run(-move_speed);
  motor_right.run(move_speed/10);
}

void move_not() {
  motor_left.run(0);
  motor_right.run(0);
}

void move_random() {

  randomSeed(analogRead(6));
  uint8_t randNumber = random(2);

  switch (randNumber) {
  case 0:
    move_right();
    delay(600);
    break;
  case 1:
    move_left();
    delay(600);
    break;
  }
}

void check_button() {
  if((0^(analogRead(A7)>10?0:1))){
    if (state > 0) {
      state = 0;
      mode = MODE_A;
      delay(200);
    } else {
      state = 1;
      mode = MODE_B;
      delay(200);
    }
    while(!((1^(analogRead(A7)>10?0:1)))){}
  }
}

void draw_warning() {
  matrix.setColorIndex(1);
  matrix.setBrightness(6);
  draw_temp = new unsigned char[16]{0,0,0,0,0,0,0,223,223,0,0,0,0,0,0,0};
  memcpy(draw_buffer,draw_temp,16);
  free(draw_temp);
  matrix.drawBitmap(0,0,16,draw_buffer);
}

void draw_question() {
  matrix.setColorIndex(1);
  matrix.setBrightness(6);
  draw_temp = new unsigned char[16]{0,0,0,0,0,6,15,217,217,3,6,0,0,0,0,0};
  memcpy(draw_buffer,draw_temp,16);
  free(draw_temp);
  matrix.drawBitmap(0,0,16,draw_buffer);
}
