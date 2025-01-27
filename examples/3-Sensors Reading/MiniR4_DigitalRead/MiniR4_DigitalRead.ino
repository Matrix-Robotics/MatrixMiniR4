/*
  Matrix Mini R4 Digital Example
 * Description: Demonstrates how to read the value of the digital signal on Matrix Mini R4.

 * Author: Barry
 * Modified 9 Oct 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMATRIX Mini R4 Test - Digital Read\n");
  Serial.println("Starting Up ... \n");
  delay(1000);
}

void loop() {
  int DI1L = MiniR4.D1.getL();
  int DI1R = MiniR4.D1.getR();

  int DI2L = MiniR4.D2.getL();
  int DI2R = MiniR4.D2.getR();

  int DI3L = MiniR4.D3.getL();
  int DI3R = MiniR4.D3.getR();

  int DI4L = MiniR4.D4.getL();
  int DI4R = MiniR4.D4.getR();

  char buff[80];
  sprintf(buff, "DI1: %d, %d, DI2: %d, %d, DI3: %d, %d, DI4: %d, %d", DI1L, DI1R, DI2L, DI2R, DI3L, DI3R, DI4L, DI4R);
  Serial.println(buff);

  delay(100);

}
