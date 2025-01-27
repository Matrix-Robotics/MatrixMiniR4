/*
  Matrix Mini R4 I2C - MATRIX Gesture Sensor Example
 * Description: Demonstrates how to read the MATRIX Gesture Sensor PAJ7620 using I2C on the Matrix Mini R4.

 * Author: Anthony
 * Modified 12 Jan 2025

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMatrix Mini R4 Test - I2C MATRIX Gesture Sensor\n");
  Serial.println("Starting Up ... \n");
  while (MiniR4.I2C4.MXGesture.begin() != 0) {
    Serial.println("Initial MATRIX Gesture Sensor failure! Please check if all the wire connections are fine, or if the wire sequence is correct?");
    delay(500);
  }
  // True: fast mode (default) can recognize 9 gestures, False: slow mode will have 4 expanded gestures.
  // MiniR4.I2C1.MXGesture.setGestureHighRate(false); 
  delay(1000);
}

void loop() {
  /* getGesture() return number and it means:
    0: None, 1: Right, 2: Left, 3: Up, 4: Down, 5: Forward, 6: Backward
    7: Clockwise, 8: Anti-Clockwise, 9: Wave
  */
  int gesture = MiniR4.I2C4.MXGesture.getGesture();
  if (gesture != 0) {
    String description = MiniR4.I2C4.MXGesture.gestureDescription(gesture);  //Convert gesture code number into string description
    Serial.print("Gesture code = ");
    Serial.println(gesture);
    Serial.print("Gesture DESC = ");
    Serial.println(description);
    Serial.println();
  }
}
