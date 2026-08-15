/*
  Matrix Mini R4 I2C - MATRIX MJ5 Example
 * Description: Demonstrates how to read the MATRIX MJ5 Joystick using I2C Port2 on the Matrix Mini R4.

 * Author: Anthony
 * Modified 15 Aug 2026

  www.matrixrobotics.com
*/
#include "MatrixMiniR4.h"

void setup()
{
  MiniR4.begin();
  Serial.begin(9600);
  MiniR4.I2C2.MJ5.begin();
  delay(500);
}

void loop()
{
  MiniR4.I2C2.MJ5.update();

  if (!MiniR4.I2C2.MJ5.isConnected()) {
    Serial.println("PS2 not connected");
    delay(500);
    return;
  }

  Serial.print("LX:"); Serial.print(MiniR4.I2C2.MJ5.getLX());
  Serial.print(" LY:"); Serial.print(MiniR4.I2C2.MJ5.getLY());
  Serial.print(" RX:"); Serial.print(MiniR4.I2C2.MJ5.getRX());
  Serial.print(" RY:"); Serial.print(MiniR4.I2C2.MJ5.getRY());
  Serial.print(" BTN:0x"); Serial.println(MiniR4.I2C2.MJ5.getButtons(), HEX);

  delay(1);
}