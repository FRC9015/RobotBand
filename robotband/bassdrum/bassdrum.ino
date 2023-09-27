#include <Servo.h>
//#include <VarSpeedServo.h>

// Variable for the single connected server.
Servo my_servo;

// Global variables (program settings)
int pos = 0; // Variable for current position
int max_angle = 90; // Furthest motor should turn (between 0 and 180, inclusive)
int rate = 5; // Delay (in ms) for simple=false
int simple_rate = 500;  // Delay (in ms) for simple=true
bool simple = false; // Runs the simplified version of the servo rotation code if true, if false runs the slightly more complex version of the code
bool off = true;

void setup() {
  // put your setup code here, to run once:
  my_servo.attach(9);
  my_servo.write(0);
  delay(1000);
}

void loop() {
  //put your main code here, to run repeatedly:

  if (off) {
    return;
  }

  if (simple) {
    my_servo.write(max_angle);
    delay(simple_rate);
    my_servo.write(0);
    delay(simple_rate);
  } else {    
    for (pos = 0; pos <= max_angle; pos += 1) {
      my_servo.write(pos);
      delay(rate);
    }
    for (pos = max_angle; pos >= 30; pos -= 1) {
      my_servo.write(pos);
      delay(rate);
    }
  }
}
