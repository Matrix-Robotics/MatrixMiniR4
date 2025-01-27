/*
  Matrix Mini R4 Motor Example
 * Description: Demonstrates how to use Matrix Mini R4 to control four DC motors.

 * Author: Barry
 * Modified 9 Oct 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMatrix Mini R4 Test - DC Motor\n");
  Serial.println("Starting Up ... ");
  MiniR4.M1.setBrake(true); // Motor brake
  MiniR4.M2.setBrake(true);
  MiniR4.M3.setBrake(true);
  MiniR4.M4.setBrake(true);
  delay(1000);
}

void loop() {
  int i;

  for (i = 0; i < 100; i++) {
    MiniR4.M1.setPower(i);
    MiniR4.M2.setPower(i);
    MiniR4.M3.setPower(i);
    MiniR4.M4.setPower(i);
    delay(15);
  }
  for (i = 100; i > 0; i--) {
    MiniR4.M1.setPower(i);
    MiniR4.M2.setPower(i);
    MiniR4.M3.setPower(i);
    MiniR4.M4.setPower(i);
    delay(15);
  }
  
  MiniR4.M1.setBrake(true); // Motor brake
  MiniR4.M2.setBrake(true);
  MiniR4.M3.setBrake(true);
  MiniR4.M4.setBrake(true);
  delay(500);

  for (i = 0; i > -100; i--) {
    MiniR4.M1.setPower(i);
    MiniR4.M2.setPower(i);
    MiniR4.M3.setPower(i);
    MiniR4.M4.setPower(i);
    delay(15);
  }
  for (i = -100; i < 0; i++) {
    MiniR4.M1.setPower(i);
    MiniR4.M2.setPower(i);
    MiniR4.M3.setPower(i);
    MiniR4.M4.setPower(i);
    delay(15);
  }
  
  MiniR4.M1.setBrake(true); // Motor brake
  MiniR4.M2.setBrake(true);
  MiniR4.M3.setBrake(true);
  MiniR4.M4.setBrake(true);
  delay(500);
}
