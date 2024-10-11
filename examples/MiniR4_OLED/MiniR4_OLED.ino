/*
  Matrix Mini R4 OLED Example
 * Description: Demonstrates how to show the Battery voltage and percent on the Matrix Mini R4 OLED screen.

 * Author: Barry
 * Modified 9 Oct 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2

  MiniR4.OLED.clearDisplay(); // Clear screen
  MiniR4.OLED.setTextSize(2); // Font size
  MiniR4.OLED.setCursor(5, 5);  // position
  MiniR4.OLED.print("Starting..");  // contents
  MiniR4.OLED.display();  // display
  delay(1000);
  MiniR4.OLED.setTextSize(1);
}

void loop() {
  float voltage = MiniR4.PWR.getBattVoltage();  // Read the Voltage of Matrix Mini R4
  float percentage = MiniR4.PWR.getBattPercentage();  // Battery percentage

  MiniR4.OLED.clearDisplay();
  MiniR4.OLED.setCursor(5, 5);
  MiniR4.OLED.print("Vol:" + String(voltage) + "V");
  MiniR4.OLED.setCursor(5, 20);
  MiniR4.OLED.print("Per:" + String(percentage) + "%");
  MiniR4.OLED.display();

  delay(100);
}
