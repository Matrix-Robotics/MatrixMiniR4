/*
  Matrix Mini R4 Buzzer Example
 * Description: A simple application with Buzzer.

 * Author: Anthony
 * Modified 12 Jan 2025

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMatrix Mini R4 Test - Buzzer\n");
  Serial.println("Starting Up ... ");
  delay(1000);
}

void loop() {
  MiniR4.Buzzer.Tone(262, 1000); //non-blocking method, will execute the next piece of code.
  delay(1000); //due to non-blocking, we need add a wait here.
  MiniR4.Buzzer.NoTone(); //non-blocking method, will execute the next piece of code.
  delay(1000); //due to non-blocking, we need add a wait here.
}
