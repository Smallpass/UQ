#include <AFMotor.h>

#include <Servo.h>

#include <SoftwareSerial.h>



Servo servo1;  // create servo object for servo 1
Servo servo2;  // create servo object for servo 2

int pos1 = 90;  // variable to store servo 1 position
int pos2 = 90;  // variable to store servo 2 position

AF_DCMotor motor1(1);  // create motor object for motor 1
AF_DCMotor motor2(2);  // create motor object for motor 2
AF_DCMotor motor4(4);








void setup() {
  servo1.attach(9);    // attach servo 1 to pin 9
  servo2.attach(10);   // attach servo 2 to pin 10

  Serial.begin(9600);      // make sure your Serial Monitor is also set at this baud rate.
 }

void loop() {

  // read incoming serial command
  if (Serial.available() > 0) {
    char command = Serial.read();

  if (command == 't'){
  motor4.run(FORWARD);
  motor4.setSpeed(1000);
  delay(250);
  motor4.run(RELEASE);
}


    // control both motors
    if (command == 's') {  // move both motors forward
      motor1.run(FORWARD);
      motor1.setSpeed(150);  // set motor speed to 150 (out of 255)
      motor2.run(FORWARD);
      motor2.setSpeed(150);
      delay(250);
      motor1.run(RELEASE);  // release motors when 'w' is no longer held down
      motor2.run(RELEASE);
    } else if (command == 'w') {  // move both motors backward
      motor1.run(BACKWARD);
      motor1.setSpeed(150);
      motor2.run(BACKWARD);
      motor2.setSpeed(150);
      delay(250);
      motor1.run(RELEASE);  // release motors when 's' is no longer held down
      motor2.run(RELEASE);
    }
  }
  // read incoming serial command
  if (Serial.available() > 0) {
    char command = Serial.read();

    // control servo 1
    if (command == 'i') {         // move servo 1 up
      pos1 = min(180, pos1 + 5);  // increase servo 1 position by 5, up to a maximum of 180
      servo1.write(pos1);         // set servo 1 position
    } else if (command == 'k') {  // move servo 1 down
      pos1 = max(0, pos1 - 5);    // decrease servo 1 position by 5, down to a minimum of 0
      servo1.write(pos1);         // set servo 1 position
    }

    // control servo 2
    else if (command == 'l') {    // move servo 2 left
      pos2 = max(0, pos2 - 5);    // decrease servo 2 position by 5, left to a minimum of 0
      servo2.write(pos2);         // set servo 2 position
    } else if (command == 'j') {  // move servo 2 right
      pos2 = min(180, pos2 + 5);  // increase servo 2 position by 5, right to a maximum of 180
      servo2.write(pos2);         // set servo 2 position
    }
  }
}
