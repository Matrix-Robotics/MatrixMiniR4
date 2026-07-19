/*
  Matrix Motor Controller DC Motor Robot Drving in a Square Example
 * Description: Demonstrates how to use Motor Controller to control four DC motors.

 * Author: Barry
 * Modified 19 Jul 2026

  www.matrixrobotics.com
*/

#include <MatrixMiniR4.h>

void runSegment(int segNum) {
  MiniR4.MXMotorCtrl.MC1.M1.resetCounter();
  MiniR4.MXMotorCtrl.MC1.M2.resetCounter();

  MiniR4.MXMotorCtrl.MC1.M1.setPower(30);
  MiniR4.MXMotorCtrl.MC1.M2.setPower(30);

  while (true) {
    int m1 = MiniR4.MXMotorCtrl.MC1.M1.getDegrees();
    int m2 = MiniR4.MXMotorCtrl.MC1.M2.getDegrees();
    if ((abs(m1) + abs(m2)) / 2 >= 500) break;
  }

  MiniR4.MXMotorCtrl.MC1.M1.setPower(0);
  MiniR4.MXMotorCtrl.MC1.M2.setPower(0);

  int m1Final = MiniR4.MXMotorCtrl.MC1.M1.getDegrees();
  int m2Final = MiniR4.MXMotorCtrl.MC1.M2.getDegrees();

  Serial.print("Segment ");
  Serial.print(segNum);
  Serial.print(" | M1: ");
  Serial.print(m1Final);
  Serial.print(" | M2: ");
  Serial.print(m2Final);
  Serial.print(" | Avg: ");
  Serial.println(abs(m1Final) + abs(m2Final) / 2);

  delay(500);
}

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);
  MiniR4.MXMotorCtrl.begin(true, false, false, false);

  MiniR4.MXMotorCtrl.MC1.M1.setReverse(false);
  MiniR4.MXMotorCtrl.MC1.M2.setReverse(true);

  MiniR4.MXMotorCtrl.MC1.M1.setPPR(400);
  MiniR4.MXMotorCtrl.MC1.M2.setPPR(400);

  for (int i = 1; i <= 4; i++) {
    runSegment(i);
  }

  Serial.println("Done.");
}

void loop() {
}
