/*
  Matrix Mini R4 Empty Template
 * Description: A Empty Template, you can start coding with this template.

 * Author: Anthony
 * Modified 11 Nov 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  //Init the Mini R4 Controller.
  MiniR4.begin();
  MiniR4.PWR.setBattCell(2);
  Serial.begin(115200);
  
}

void loop() {
  //User Code.
  
}
