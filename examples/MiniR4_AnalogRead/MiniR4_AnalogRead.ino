/*
  Matrix Mini R4 Analog Example
 * Description: Demonstrates how to read the value of the analog signal on Matrix Mini R4.

 * Author: Barry
 * Modified 9 Oct 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  Serial.println("\nMatrix Mini R4 Test - Analog Read\n");
  Serial.println("Starting Up ... \n");
  delay(1000);
}

void loop() {
  int AI1L = MiniR4.A1.getAIL();
  int AI1R = MiniR4.A1.getAIR();

  int AI2L = MiniR4.A2.getAIL();
  int AI2R = MiniR4.A2.getAIR();

  int AI3L = MiniR4.A3.getAIL();
  int AI3R = MiniR4.A3.getAIR();

  char buff[80];
  sprintf(buff, "AI1: %d, %d, AI2: %d, %d, AI3: %d, %d", AI1L, AI1R, AI2L, AI2R, AI3L, AI3R);
  Serial.println(buff);

  delay(100);
}
