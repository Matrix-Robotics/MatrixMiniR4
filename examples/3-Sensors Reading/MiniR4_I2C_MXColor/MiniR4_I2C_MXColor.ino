/*
  Matrix Mini R4 I2C - MATRIX Color Sensor Example
 * Description: Demonstrates how to read the MATRIX Color Sensor using I2C on the Matrix Mini R4.
 * Notice: This is for MATRIX Color Sensor V1 V2, If you using MATRIX Color Sensor with "V3" name (New one), please using MXColorV3 function instead.

 * Author: Barry
 * Modified 9 Oct 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMatrix Mini R4 Test - I2C MATRIX Color Sensor\n");
  Serial.print("\n!! This is for MATRIX Color Sensor V2, If you using MATRIX Color Sensor with 'V3' name (New one)");
  Serial.println("\n, please using MXColorV3 function instead. !!\n");
  Serial.println("Starting Up ... \n");
  delay(1000);
}

void loop() {
  int r = MiniR4.I2C1.MXColor.getColor(R);
  int g = MiniR4.I2C1.MXColor.getColor(G);
  int b = MiniR4.I2C1.MXColor.getColor(B);
  int c = MiniR4.I2C1.MXColor.getColor(C);
  int m = MiniR4.I2C1.MXColor.getColor(M);
  int y = MiniR4.I2C1.MXColor.getColor(Y);
  int k = MiniR4.I2C1.MXColor.getColor(K);

  char buff[64];
  sprintf(buff, "R: %3d, G: %3d, B: %3d, C: %3d, M: %3d, Y: %3d, K: %3d\n", r, g, b, c, m, y, k);
  Serial.print(buff);

  delay(100);
}
