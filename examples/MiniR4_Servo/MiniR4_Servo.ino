/*
  Matrix Mini R4 Servo Motor Example
 * Description: Demonstrates how to use the Matrix Mini R4 to control four Servo motors.

 * Author: Barry
 * Modified 9 Oct 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

int angle = 0;

void setup() {
  MiniR4.begin();
  MiniR4.PWR.setBattCell(2); //18650 battery x 2
  Serial.begin(115200);
  Serial.println("\nMatrix Mini R4 Test - Servo Motor\n");
  Serial.println("Starting Up ... ");

  MiniR4.RC1.setAngle(angle);
  MiniR4.RC2.setAngle(angle);
  MiniR4.RC3.setAngle(angle);
  MiniR4.RC4.setAngle(angle);

  delay(1000);
}

void loop() {
  delay(50);
  angle += 5;
  MiniR4.RC1.setAngle(angle);
  MiniR4.RC2.setAngle(angle);
  MiniR4.RC3.setAngle(angle);
  MiniR4.RC4.setAngle(angle);
  if (angle > 180) angle = 0;
}
