#include <Adafruit_PWMServoDriver.h>
#include "MIDIUSB.h"


#define SERVOMIN 150    //this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 500    //this is the 'maximum' pulse length count (out or 4096)
#define PRESSANGLE 100  //where the servo should be for the key to be pressed
#define STALLANGLE 250  //where the servo should be when it doesn't want to press anything

#define I2C_SERVOS 16
#define TOTAL_I2C 4



Adafruit_PWMServoDriver pwm[4] = {
  Adafruit_PWMServoDriver(0x40),
  Adafruit_PWMServoDriver(0x41),
  Adafruit_PWMServoDriver(0x42),
  Adafruit_PWMServoDriver(0x43)};
int invertedKeys[7] = {0,4,7,11,14,17,21};
int blackKeys[5] = {1,3,6,8,10};
// need to figure out if we are going ot have this big array containing all the information or if we will use the index of the note to find out the address of the servo or if we just index through the array until we find the right note or if we want to have multiple arrays for each individual thing that can correspond to address,note,and num or smth



// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("Starting!");
  for(int i = 0; i < 4; i++){
    pwm[i].begin();
    pwm[i].setPWMFreq(50);
    for(int j = 0;j<16;j++){
      pwm[i].setPWM(j,0,STALLANGLE);
    }
  }

  
}

bool isInverted(int index) {
  int newIndex = index % 24;
  for (int i = 0; i < 14; i++){
    if (invertedKeys[i] == newIndex){
      return true;
    }
  }
  return false;
}
bool isBlackKey(int index){
  int newIndex = index % 12;
  for (int i = 0; i < 5; i++){
    if (blackKeys[i] == newIndex){
      return true;
    }
  }
  return false;
}

void pressNote(midiEventPacket_t r) {
  int index = r.byte2 - 36; //account for missing octaves from the midi input
  int pressing = index % I2C_SERVOS;
  int pwmNumber = index / I2C_SERVOS;
  if (r.byte2 <= 96 && r.byte2 >= 36) {
    pwm[pwmNumber].setPWM(pressing, 0, (isBlackKey(index)) ? SERVOMIN : (isInverted(index)) ? STALLANGLE : PRESSANGLE);
    Serial.print(index);
    Serial.print(" Servo: ");
    Serial.print(pressing);
    Serial.println(" Press");
  }
}
void releaseNote(midiEventPacket_t r) {
  int index = r.byte2 - 36;
  int pressing = index % I2C_SERVOS;
  int pwmNumber = index / I2C_SERVOS;
  if (r.byte2 <= 96 && r.byte2 >= 36) {
    pwm[pwmNumber].setPWM(pressing, 0, (isBlackKey(index)) ? SERVOMAX : (isInverted(index)) ? PRESSANGLE : STALLANGLE);
    Serial.print(index);
    Serial.print(" Servo: ");
    Serial.print(index % I2C_SERVOS);
    Serial.println(" Release");
  }
}
void cycle(){
  for(int i = 0;i<16;i++){
    pwm[0].setPWM(i,0,PRESSANGLE);
    delay(1000);
  }
  for(int i = 0; i < 16; i++){
    pwm[0].setPWM(i,0,STALLANGLE);
    delay(1000);
  }
}
void cycleServo(int servonum){
  pwm[0].setPWM(servonum,0,SERVOMIN);
  delay(500);
  pwm[0].setPWM(servonum,0,SERVOMAX);
  delay(500);
}


void loop() {
  //put your main code here, to run repeatedly:
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if ((0x0f & rx.byte1) == 0 && rx.byte3 == 0x64) {
      Serial.println("press");
      //send back the received MIDI command
      pressNote(rx);  
    } else {
      releaseNote(rx);      
    }
  } while (rx.header != 0);
  
}