#include <Adafruit_PWMServoDriver.h>
#include "MIDIUSB.h"


#define SERVOMIN 150    //this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 500    //this is the 'maximum' pulse length count (out or 4096)
#define PRESSANGLE 100  //where the servo should be for the key to be pressed
#define STALLANGLE 250  //where the servo should be when it doesn't want to press anything

#define I2C_SERVOS 16
#define TOTAL_I2C 4



Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// need to figure out if we are going ot have this big array containing all the information or if we will use the index of the note to find out the address of the servo or if we just index through the array until we find the right note or if we want to have multiple arrays for each individual thing that can correspond to address,note,and num or smth



// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void setup() {
  // put your setup code here, to run once:
  // for(uint8_t i; i < sizeof(servos); i++){
  //   if(isDigit(servos[i].charAt(2))){

  //   } else if(isDigit(servos[i].charAt(1))){

  //   } else{
  //     Serial.println("There is an Error in the notes!");
  //     break;
  //   }
  // }

  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50);  //The analog servo frequency is about 50Hz

  
}

// void pressNote(float noteDuration, int note, float tempo){
//   pwm.setPWM(note, 0, PRESSANGLE);
//   delay(noteDuration * 1000 * 60/tempo);
//   pwm.setPWM(note, 0, STALLANGLE);
// }


// void cycleServo(uint8_t servonum, float noteDuration, float tempo){
//   pwm.setPWM(servonum, 0, SERVOMIN);
//   delay(noteDuration * 1000 * 60/tempo);
//   pwm.setPWM(servonum, 0, SERVOMAX);
//   delay(noteDuration * 1000 * 60/tempo);
// }
void pressNote(midiEventPacket_t r) {
  int index = r.byte2 - 37;
  int pressing = index % I2C_SERVOS;
  if (r.byte2 <= 96 && r.byte2 >= 36) {
    if (pressing % 2 != 0){
      pwm.setPWM(pressing, 0, PRESSANGLE);
    } else { 
      pwm.setPWM(pressing, 0, STALLANGLE);
    }
    Serial.print(index);
    Serial.print(" Servo: ");
    Serial.print(pressing);
    Serial.println(" Press");
  }
}
void releaseNote(midiEventPacket_t r) {
  int index = r.byte2 - 37;
  int pressing = index % I2C_SERVOS;
  if (r.byte2 <= 96 && r.byte2 >= 36) {
    if (pressing % 2 != 0){
      pwm.setPWM(pressing, 0, STALLANGLE);
    } else { 
      pwm.setPWM(pressing, 0, PRESSANGLE);
    }
    Serial.print(index);
    Serial.print(" Servo: ");
    Serial.print(index % I2C_SERVOS);
    Serial.println(" Release");
  }
}
void cycle(){
  for(int i = 0;i<16;i++){
    pwm.setPWM(i,0,PRESSANGLE);
    delay(1000);
  }
  for(int i = 0; i < 16; i++){
    pwm.setPWM(i,0,STALLANGLE);
    delay(1000);
  }
}
void cycleServo(int servonum){
  pwm.setPWM(servonum,0,PRESSANGLE);
  delay(500);
  pwm.setPWM(servonum,0,STALLANGLE);
  delay(500);
}


void loop() {
  //put your main code here, to run repeatedly:
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if ((0x0f & rx.byte1) == 0 && rx.byte3 == 0x64) {
      //send back the received MIDI command
      pressNote(rx);  
    } else {
      releaseNote(rx);      
    }
  } while (rx.header != 0);
  
}
