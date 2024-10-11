/*
  Matrix Mini R4 IMU Example
 * Description: Demonstrates how to read the Eular angle using IMU on the Matrix Mini R4.

 * Author: Barry
 * Modified 9 Oct 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  Serial.println("\nMATRIX Mini R4 Test - IMU Read\n");
  Serial.println("Starting Up ... \n");

  MiniR4.Motion.resetIMUValues(); // Reset IMU

  delay(1000);
}

void loop() {
  int roll  = MiniR4.Motion.getEuler(MiniR4Motion::AxisType::Roll);
  int pitch = MiniR4.Motion.getEuler(MiniR4Motion::AxisType::Pitch);
  int yaw   = MiniR4.Motion.getEuler(MiniR4Motion::AxisType::Yaw);

  char buff[80];
  sprintf(buff, "Euler: r=%3d, p=%3d, y=%3d\t\n", roll, pitch, yaw);
  Serial.print(buff);

  delay(100);
}
