/*
  Matrix Motor Controller DC Motor Example
 * Description: Demonstrates how to use Motor Controller to control four RC motors.

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
  
  MiniR4.MXMotorCtrl.MC1.RC1.setReverse(true);
  MiniR4.MXMotorCtrl.MC1.RC2.setReverse(false);

}

void loop() {

  for (int i = 0; i <= 180; i++) {
    MiniR4.MXMotorCtrl.MC1.RC1.setAngle(i);
    MiniR4.MXMotorCtrl.MC1.RC2.setAngle(180 - i);

    Serial.print("RC1 Angle: "); Serial.print(i);
    Serial.print("  |  RC2 Angle: "); Serial.println(180 - i);

    delay(10);
  }

  for (int i = 180; i >= 0; i--) {
    MiniR4.MXMotorCtrl.MC1.RC1.setAngle(i);
    MiniR4.MXMotorCtrl.MC1.RC2.setAngle(180 - i);

    Serial.print("RC1 Angle: "); Serial.print(i);
    Serial.print("  |  RC2 Angle: "); Serial.println(180 - i);

    delay(10);
  }

}
