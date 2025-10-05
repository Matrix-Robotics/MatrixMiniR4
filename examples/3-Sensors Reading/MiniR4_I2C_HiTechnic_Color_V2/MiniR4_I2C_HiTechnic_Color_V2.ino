/**
 * @file MiniR4_I2C_HiTechnic_Color_V2.ino
 * @brief Example usage of HiTechnic NXT Color Sensor V2 with MiniR4
 * @author MATRIX Robotics
 */

#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  MiniR4.PWR.setBattCell(2);
  Serial.begin(9600);

  // HiTechnic NXT Color Sensor V2 Begin
  MiniR4.I2C1.HTCol.begin();

  Serial.println("=== MiniR4 HT Color Sensor V2 Example ===");
  delay(1000);
}

void loop() {
  Serial.println("==========================================");

  // Method 1: Get individual values
  uint8_t colorNum = MiniR4.I2C1.HTCol.getColorNumber();
  uint8_t red = MiniR4.I2C1.HTCol.getR();
  uint8_t green = MiniR4.I2C1.HTCol.getG();
  uint8_t blue = MiniR4.I2C1.HTCol.getB();

  Serial.print("Color Number: ");
  Serial.println(colorNum);
  Serial.print("RGB Values - R: ");
  Serial.print(red);
  Serial.print(", G: ");
  Serial.print(green);
  Serial.print(", B: ");
  Serial.println(blue);

  // Method 2: Get RGB values together
  uint8_t r, g, b;
  MiniR4.I2C1.HTCol.getRGB(r, g, b);
  Serial.print("RGB (batch): R:");
  Serial.print(r);
  Serial.print(", G:");
  Serial.print(g);
  Serial.print(", B:");
  Serial.println(b);

  // Method 3: Get HSV values together
  float h, s, v;
  MiniR4.I2C1.HTCol.getHSV(h, s, v);
  Serial.print("HSV (batch): H:");
  Serial.print(h, 2);
  Serial.print("°, S:");
  Serial.print(s, 2);
  Serial.print("%, V:");
  Serial.print(v, 2);
  Serial.println("%");

  delay(100);
}