/*
  Matrix Mini R4 - MATRIX One Wire Temperature Sensor Example
 * Description: Demonstrates how to read the MATRIX One Wire Temperature Sensor (DS18B20) using One Wire (DI) on the Matrix Mini R4.

 * Author: Anthony
 * Modified 12 Jan 2025

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMatrix Mini R4 Test - MATRIX One Wire Temperature Sensor\n");
  Serial.println("Starting Up ... \n");
  delay(1000);
}

void loop() {
  float temp = MiniR4.D1.MXOnewireDT.requestAndGetTemp();
  Serial.print("Temp: ");
  Serial.println(temp);

  delay(100);
}
