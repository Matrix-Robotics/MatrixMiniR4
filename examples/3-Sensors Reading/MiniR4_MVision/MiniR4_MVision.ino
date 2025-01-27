/*
  Matrix Mini R4 M-Vision Example
 * Description: Demonstration of basic usage of M-Vision

 * Author: Barry
 * Modified 17 Nov 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

unsigned int CamData[10];
int CamStatus;

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)

  MiniR4.Vision.Begin();  // Initialize M-Vision

}

void loop() {
  CamStatus = MiniR4.Vision.SmartCamReader(CamData); // Read M-Vision data

  if (CamStatus > 0) {
    Serial.println(CamData[0]);
  } else {
    Serial.println("None");
  }

  delay(100);

}
