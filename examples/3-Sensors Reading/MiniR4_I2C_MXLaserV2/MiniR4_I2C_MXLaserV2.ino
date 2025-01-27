/*
  Matrix Mini R4 I2C - MATRIX Laser Sensor V2 Example
 * Description: Demonstrates how to read the MATRIX Laser Sensor V2 using I2C on the Matrix Mini R4.
 * Notice: This is for MATRIX Laser Sensor V2, If you using MATRIX Laser Sensor without "V2" name (Old one), please using MXLaser function instead.

 * Author: Anthony
 * Modified 12 Jan 2025

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMatrix Mini R4 Test - I2C MATRIX Laser Sensor V2\n");
  Serial.print("\n!! This is for MATRIX Laser Sensor V2, If you using MATRIX Laser Sensor without 'V2' name (Old one)");
  Serial.println("\n, please using MXLaser function instead. !!\n");
  if (!MiniR4.I2C1.MXLaserV2.begin())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  Serial.println("Starting Up ... \n");
  delay(1000);
}

void loop() {
  int dis = MiniR4.I2C1.MXLaserV2.getDistance();
  Serial.print("Distance = ");
  Serial.print(dis);
  Serial.println("mm");
  delay(100);
}
