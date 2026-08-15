/*
  Matrix Mini R4 MJ5 Control Example
 * Description: Demonstrates how to use MJ5 to wirelessly control the Matrix Mini R4 robot.
 * Note: Please Connect MJ5 Joystick to I2C2.

 * Author: Anthony
 * Modified 14 Aug 2026

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

int PWR;
int STR;

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMATRIX Mini R4 Test - PS2 Motor Control\n");
  Serial.println("Starting Up ... \n");

  MiniR4.M1.setReverse(true);
  MiniR4.M2.setReverse(false);

  MiniR4.M1.setBrake(0);
  MiniR4.M2.setBrake(0);
  
  MiniR4.I2C2.MJ5.begin();
  delay(1000);
}

void loop() {
  MiniR4.I2C2.MJ5.update();  // Read the Joystick
  PWR = map(MiniR4.I2C2.MJ5.getLY(), 0, 255, 100, (-100));
  STR = map(MiniR4.I2C2.MJ5.getRX(), 0, 255, (-100), 100);

  MiniR4.M1.setPower((PWR + STR));
  MiniR4.M2.setPower((PWR - STR));

  delay(10);
}
