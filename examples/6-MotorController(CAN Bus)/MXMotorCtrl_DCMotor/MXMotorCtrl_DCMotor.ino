/*
  Matrix Motor Controller DC Motor Example
 * Description: Demonstrates how to use Motor Controller to control four DC motors.

 * Author: Barry
 * Modified 19 Jul 2026

  www.matrixrobotics.com
*/

#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin(); // Initialize the Matrix Mini R4 library
  Serial.begin(115200); // Set Baud rate
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  MiniR4.MXMotorCtrl.begin(true, false, false, false);
  
  MiniR4.MXMotorCtrl.MC1.M1.setReverse(true);
  MiniR4.MXMotorCtrl.MC1.M2.setReverse(false);

  MiniR4.MXMotorCtrl.MC1.M1.setPPR(400);
  MiniR4.MXMotorCtrl.MC1.M2.setPPR(400);

  MiniR4.MXMotorCtrl.MC1.M1.resetCounter();
  MiniR4.MXMotorCtrl.MC1.M2.resetCounter();
}

void loop() {
  MiniR4.MXMotorCtrl.MC1.M1.setPower(50);
  MiniR4.MXMotorCtrl.MC1.M2.setPower(50);

  int M1ENC = MiniR4.MXMotorCtrl.MC1.M1.getDegrees();
  int M2ENC = MiniR4.MXMotorCtrl.MC1.M2.getDegrees();
  Serial.print("M1 ENC: "); Serial.print(M1ENC);
  Serial.print("  |  M2 ENC: "); Serial.println(M2ENC);

  delay(10);
}
