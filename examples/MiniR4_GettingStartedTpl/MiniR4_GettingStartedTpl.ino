/*
  Matrix Mini R4 Getting Started Template
 * Description: Demonstration of basic program

 * Author: Barry
 * Modified 17 Nov 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin(); // Initialize the Matrix Mini R4 library
  Serial.begin(115200); // Set Baud rate
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)

  // put your setup code here, to run once:

}

void loop() {

  // put your main code here, to run repeatedly:

}
