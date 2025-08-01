#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

#include <AFMotor.h>

#include <Servo.h>

#include <SoftwareSerial.h>
SoftwareSerial HM10(2, 3); // RX = 2, TX = 3


Servo servo1;  // create servo object for servo 1
Servo servo2;  // create servo object for servo 2

int pos1 = 90;  // variable to store servo 1 position
int pos2 = 90;  // variable to store servo 2 position

AF_DCMotor motor1(1);  // create motor object for motor 1
AF_DCMotor motor2(2);  // create motor object for motor 2
AF_DCMotor motor3(3);




void setup() {
  servo1.attach(9);    // attach servo 1 to pin 9
  servo2.attach(10);   // attach servo 2 to pin 10

  Serial.begin(9600);      // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin(9600);
 }


void loop() {
Dabble.processInput();             //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  Serial.print("KeyPressed: ");
  if (GamePad.isUpPressed())
  {
    motor1.run(FORWARD);
      motor1.setSpeed(150);  // set motor speed to 150 (out of 255)
      motor2.run(FORWARD);
      motor2.setSpeed(150);
  }

  if (GamePad.isDownPressed())
  {
    motor1.run(BACKWARD);
      motor1.setSpeed(150);
      motor2.run(BACKWARD);
      motor2.setSpeed(150);
  }


  if (GamePad.isSquarePressed())
  {
    pos2 = min(180, pos2 + 5);  // increase servo 2 position by 5, right to a maximum of 180
      servo2.write(pos2); 
  }

  if (GamePad.isCirclePressed())
  {
    pos2 = max(0, pos2 - 5);    // decrease servo 2 position by 5, left to a minimum of 0
      servo2.write(pos2);
  }

  if (GamePad.isCrossPressed())
  {
    pos1 = max(0, pos1 - 5);    // decrease servo 1 position by 5, down to a minimum of 0
      servo1.write(pos1);         // set servo 1 position
  }

  if (GamePad.isTrianglePressed())
  {
    pos1 = min(180, pos1 + 5);  // increase servo 1 position by 5, up to a maximum of 180
      servo1.write(pos1);         // set servo 1 position
  }

  if (GamePad.isStartPressed())
  {
    motor3.run(FORWARD);
  motor3.setSpeed(500);
  }

  if (GamePad.isSelectPressed())
  {
  motor3.run(RELEASE);
  }
}