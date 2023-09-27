#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  125 //this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  575 //this is the 'maximum' pulse length count (out or 4096)

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

uint8_t servonum = 0;

void setup() {
  // put your setup code here, to run once:
  pwm.begin();
  pwm.setPWMFreq(60);
}

void loop() {
  // put your main code here, to run repeatedly:
  pwm.setPWM()
}
