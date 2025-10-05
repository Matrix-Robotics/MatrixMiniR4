/*
  Matrix Mini R4 I2C - MATRIX Color Sensor V3 Example
 * Description: Demonstrates how to read the MATRIX Color Sensor V3 using I2C on the Matrix Mini R4.
 * Notice: This is for MATRIX Color Sensor V3, If you using MATRIX Color Sensor without "V3" name (Old one), please using MXColor function instead.

 * Author: Anthony
 * Modified 2 Oct 2025

  www.matrixrobotics.com
*/
#include "MatrixMiniR4.h"

void setup()
{
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMatrix Mini R4 Test - I2C MATRIX Color Sensor V3\n");
  Serial.print("\n!! This is for MATRIX Color Sensor V3, If you using MATRIX Color Sensor without 'V3' name (old one)");
  Serial.println("\n, please using MXColor function instead. !!\n");
  Serial.println("Starting Up ... \n");
  MiniR4.I2C1.MXColorV3.begin();
  delay(1000);
}

void loop() {
  int r = MiniR4.I2C1.MXColorV3.getR();
  int g = MiniR4.I2C1.MXColorV3.getG();
  int b = MiniR4.I2C1.MXColorV3.getB();
  int c = MiniR4.I2C1.MXColorV3.getC();
  int h = MiniR4.I2C1.MXColorV3.getH();
  int s = MiniR4.I2C1.MXColorV3.getS();
  int v = MiniR4.I2C1.MXColorV3.getV();
  int id = MiniR4.I2C1.MXColorV3.getColorID();

  char buff[64];
  sprintf(buff, "R: %3d, G: %3d, B: %3d, C: %3d, H: %3d, S: %3d, V: %3d, ID: %3d\n", r, g, b, c, h, s, v, id);
  Serial.print(buff);

}