/*
  Matrix Mini R4 PS2 Control Example
 * Description: Demonstrates how to use MJ2 to wirelessly control the Matrix Mini R4 robot.

 * Author: Barry
 * Modified 9 Oct 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

int PWR;
int STR;

void setup() {
  MiniR4.begin();
  MiniR4.PWR.setBattCell(2);  // 18650x2
  Serial.begin(115200);
  Serial.println("\nMATRIX Mini R4 Test - PS2 Motor Control\n");
  Serial.println("Starting Up ... \n");

  MiniR4.M1.setReverse(true);
  MiniR4.M2.setReverse(false);
  MiniR4.M1.setSpeed(0);
  MiniR4.M2.setSpeed(0);

  delay(1000);
}

void loop() {
  MiniR4.PS2.read_gamepad(false, 0);  // Read the Joystick
  PWR = map(MiniR4.PS2.Analog(PSS_LY), 0, 255, 100, (-100));
  STR = map(MiniR4.PS2.Analog(PSS_RX), 0, 255, (-100), 100);

  //If using DC Motor without Encoder, please using setPower() instead of setSpeed().
  MiniR4.M1.setSpeed((pwr + str));
  MiniR4.M2.setSpeed((pwr - str));

  delay(10);
}
