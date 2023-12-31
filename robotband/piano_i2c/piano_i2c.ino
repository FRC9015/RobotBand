
#include <Servo.h>
//#include <VarSpeedServo.h>
#include <Wire.h>

// Variable for the single connected server.
// Servo my_servo;
// Global variables (program settings)
int pos = 0; // Variable for current position
int max_angle = 25; // Furthest motor should turn (between 0 and 180, inclusive)
int rate = 20; // Delay (in ms) for simple=false
int simple_rate = 500;  // Delay (in ms) for simple=true
bool simple = false; // Runs the simplified version of the servo rotation code if true, if false runs the slightly more complex version of the code
bool off = false;

// I2C global variables
bool i2c = true;

uint8_t servonum = 0;
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  Wire.setClock(400000);

  //my_servo.attach(9);
  //my_servo.write(0);
}

void loop() {
  //put your main code here, to run repeatedly:
  
  if (off) {
    return;
  }
  if (i2c){
    Wire.beginTransmission(1);
    Serial.println(Wire.available());
    Wire.endTransmission();
  }

  delay(500);
}
