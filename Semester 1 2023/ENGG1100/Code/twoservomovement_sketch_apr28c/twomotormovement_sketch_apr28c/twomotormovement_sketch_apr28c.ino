#include <AFMotor.h>

AF_DCMotor motor1(1); // create motor object for motor 1
AF_DCMotor motor2(2); // create motor object for motor 2

void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 bps
}

void loop() {
  // read incoming serial command
  if (Serial.available() > 0) {
    char command = Serial.read();

    // control both motors
    if (command == 'w') { // move both motors forward
      motor1.run(FORWARD);
      motor1.setSpeed(150); // set motor speed to 150 (out of 255)
      motor2.run(FORWARD);
      motor2.setSpeed(150);
      while (Serial.available() <= 0 || Serial.read() == 'w') { // hold 'w' to keep motors running
        motor1.run(FORWARD);
        motor1.setSpeed(150);
        motor2.run(FORWARD);
        motor2.setSpeed(150);
      }
      motor1.run(RELEASE); // release motors when 'w' is no longer held down
      motor2.run(RELEASE);
    }
    else if (command == 's') { // move both motors backward
      motor1.run(BACKWARD);
      motor1.setSpeed(150);
      motor2.run(BACKWARD);
      motor2.setSpeed(150);
      while (Serial.available() <= 0 || Serial.read() == 's') { // hold 's' to keep motors running
        motor1.run(BACKWARD);
        motor1.setSpeed(150);
        motor2.run(BACKWARD);
        motor2.setSpeed(150);
      }
      motor1.run(RELEASE); // release motors when 's' is no longer held down
      motor2.run(RELEASE);
    }
  }
}





