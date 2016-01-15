# Exemple firmwares for Makeblock mBot

Makeblock [mBot](http://www.makeblock.cc/mbot/) is an educational bot based on an arduino uno compatible board. This repository contains the default mBot firmware and some simple experiments to extend it.

## Use mBot with linux

1. install Arduino IDE from [source](https://www.arduino.cc/en/Main/Software). [Atom](https://atom.io/) and [platformio](http://platformio.org/#!/) can be used as an alternative development environment.
2. install MakeBlock 3.0 [library](https://github.com/Makeblock-official/Makeblock-Libraries)
3. get or create mBot Arduino [sketch](https://github.com/Makeblock-official/mBot/blob/master/mBot-default-program/mBot-default-program.ino) 
4. set proper write permissions on `/dev/ttyUSB0` by adding your user to the dialout group : `sudo usermod -a -G dialout $USER`
5. compile and upload code on controler board

## Pins

* A0 RJ25 plug 4 (default not-connected)
* A1 RJ25 plug 4 (default not-connected)
* A2 RJ25 plug 3 ultrasonic
* A3 RJ25 plug 3 ultrasonic
* A6 light sensor
* A7 button
* D2 IR RCV
* D3 IR TX
* D4 DIR2 - direction motor2
* D5 PWM2 - pwm motor2
* D6 PWM1 - pwn motor1
* D7 DIR1 - direction motor1
* D8 buzzer
* D9 RJ25 plug 2 linefollower
* D10 RJ25 plug 2 linefollower
* D11 RJ25 plug 1 (default not-connected)
* D12 RJ25 plug 1 (default not-connected)
* D13 2 WS2812 RGB leds

