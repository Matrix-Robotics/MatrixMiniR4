/*
  Matrix Mini R4 PS2 Control Example
 * Description: Demonstrates how to use MJ2 to wirelessly control the Matrix Mini R4 robot.
 * Note: Please Connect 4p wire to D2 (Left: DAT, Right: CMD, V, G), 2p wire to D3 (Left: CSS/SET, Right: CLK)

 * Author: Barry
 * Modified 17 Nov 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

int PWR;
int STR;

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMATRIX Mini R4 Test - PS2 Motor Control\n");
  Serial.println("Starting Up ... \n");

  MiniR4.M1.setReverse(true);
  MiniR4.M2.setReverse(false);

  MiniR4.M1.setBrake(0);
  MiniR4.M2.setBrake(0);

  delay(1000);
}

void loop() {
  MiniR4.PS2.read_gamepad(false, 0);  // Read the Joystick
  PWR = map(MiniR4.PS2.Analog(PSS_LY), 0, 255, 100, (-100));
  STR = map(MiniR4.PS2.Analog(PSS_RX), 0, 255, (-100), 100);

  MiniR4.M1.setPower((PWR + STR));
  MiniR4.M2.setPower((PWR - STR));

  delay(10);
}
