/*
  Matrix Mini R4 - MATRIX Temperature and Humidity Sensor (DHT11) Example
 * Description: Demonstrates how to read the MATRIX Temperature and Humidity Sensor (DHT11) on the Matrix Mini R4.

 * Author: Anthony
 * Modified 12 Jan 2025

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

float temp;
int hum;

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMatrix Mini R4 Test - MATRIX Temperature and Humidity Sensor\n");
  Serial.println("Starting Up ... \n");
  delay(1000);
}

void loop() {
  // temp = MiniR4.D1.MXDHT.readTemperature(); // Read Temperature only.
  // hum = MiniR4.D1.MXDHT.readHumidity(); // Read Humidity only.
  MiniR4.D1.MXDHT.readTemperatureHumidity(temp, hum); // Read Temperature and Humidity in one request.
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" Hum: ");
  Serial.println(hum);

  delay(100);
}
