#include <Servo.h>

Servo servo1;  // create servo object for servo 1
Servo servo2;  // create servo object for servo 2

int pos1 = 90; // variable to store servo 1 position
int pos2 = 90; // variable to store servo 2 position

void setup() {
  servo1.attach(9);  // attach servo 1 to pin 9
  servo2.attach(10); // attach servo 2 to pin 10
  Serial.begin(9600); // initialize serial communication at 9600 bps
}

void loop() {
  // read incoming serial command
  if (Serial.available() > 0) {
    char command = Serial.read();

    // control servo 1
    if (command == 'i') { // move servo 1 up
      pos1 = min(180, pos1 + 5); // increase servo 1 position by 5, up to a maximum of 180
      servo1.write(pos1); // set servo 1 position
    }
    else if (command == 'k') { // move servo 1 down
      pos1 = max(0, pos1 - 5); // decrease servo 1 position by 5, down to a minimum of 0
      servo1.write(pos1); // set servo 1 position
    }

    // control servo 2
    else if (command == 'j') { // move servo 2 left
      pos2 = max(0, pos2 - 5); // decrease servo 2 position by 5, left to a minimum of 0
      servo2.write(pos2); // set servo 2 position
    }
    else if (command == 'l') { // move servo 2 right
      pos2 = min(180, pos2 + 5); // increase servo 2 position by 5, right to a maximum of 180
      servo2.write(pos2); // set servo 2 position
    }
  }
}




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