/*
  Matrix Mini R4 I2C - MATRIX Line Tracer 10CH Example
 * Description: Demonstrates how to read the MATRIX Line Tracer 10CH using I2C on the Matrix Mini R4.
 * Notice: Connect to I2C0, Port A3 (current version of line tracer sensor only supports native channel connection)

 * Author: Anthony
 * Modified 12 Dec 2025

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMatrix Mini R4 Test - I2C MATRIX LineTracer\n");
  Serial.println("Starting Up ... \n");
  MiniR4.I2C0.MXLineTracer.begin(); //I2C0 (A3) Only!
  delay(1000);
}

void loop() {
  Serial.print(String("Width: ") + String(MiniR4.I2C0.MXLineTracer.getLineWidth()));
  Serial.print(String(" OnLine?: ") + String(MiniR4.I2C0.MXLineTracer.isOnline()));
  Serial.println(String(" Error: ") + String(MiniR4.I2C0.MXLineTracer.getError()));
  delay(100);
}
