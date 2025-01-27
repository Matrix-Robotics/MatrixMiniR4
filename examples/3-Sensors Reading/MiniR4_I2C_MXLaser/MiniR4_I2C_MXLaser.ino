/*
  Matrix Mini R4 I2C - MATRIX Laser Sensor Example
 * Description: Demonstrates how to read the MATRIX Laser Sensor using I2C on the Matrix Mini R4.
 * Notice: This is for MATRIX Laser Sensor, If you using MATRIX Laser Sensor with "V2" name (New one), please using MXLaserV2 function instead.

 * Author: Anthony
 * Modified 12 Jan 2025

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMatrix Mini R4 Test - I2C MATRIX Laser Sensor\n");
  Serial.print("\n!! This is for MATRIX Laser Sensor, If you using MATRIX Laser Sensor with 'V2' name (New one)");
  Serial.println("\n, please using MXLaserV2 function instead. !!\n");
  Serial.println("Starting Up ... \n");
  MiniR4.I2C1.MXLaser.begin();
  delay(1000);
}

void loop() {
  int dis = MiniR4.I2C1.MXLaser.getDistance();
  if (dis == 8191) {
    Serial.println("TIMEOUT");
  } else {
    Serial.print("Distance = ");
    Serial.print(dis);
    Serial.println("mm");
  }
  delay(100);
}
