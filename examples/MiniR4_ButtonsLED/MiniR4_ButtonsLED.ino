/*
  Matrix Mini R4 Button & LED Example
 * Description: A simple application with Buttons & LEDs.
 *               Pressed buttons to light up the LEDs.

 * Author: Barry
 * Modified 9 Oct 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  Serial.println("\nMatrix Mini R4 Test - Buttons & LED\n");
  Serial.println("Starting Up ... ");
  delay(1000);
}

void loop() {
  if (MiniR4.BTN_UP.getState()) {
    MiniR4.LED.setColor(1, 255, 0, 0);  // Red
    MiniR4.LED.setColor(2, 0, 0, 255);  // Blue
  }
  if (MiniR4.BTN_DOWN.getState()) {
    MiniR4.LED.setColor(1, 0, 0, 255);  // Blue
    MiniR4.LED.setColor(2, 255, 0, 0);  // Red
  }
  delay(10);
}
