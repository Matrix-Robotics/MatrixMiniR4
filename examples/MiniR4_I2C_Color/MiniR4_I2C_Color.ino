/*
  Matrix Mini R4 I2C Color Sensor Example
 * Description: Demonstrates how to read the Color Sensor using I2C on the Matrix Mini R4.

 * Author: Barry
 * Modified 9 Oct 2024

 * If using getColorNumber(), Number of color as follow as below:
 * 0:Black, 1:White, 2:Cyan, 3:Ocean, 4:Blue, 5:Violet, 6:Magenta
 * 7:Raspberry, 8:Red, 9:Orange, 10:Yellow, 11:Spring, 12:Green, 13:Turquoise

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  MiniR4.PWR.setBattCell(2); //18650 battery x 2
  Serial.begin(115200);
  Serial.println("\nMatrix Mini R4 Test - I2C Color Sensor\n");
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
