/*
  Matrix Mini R4 IMU Accel calibration Example
 * Description: Demonstrates how to calibrate the accelerometer using IMU on the Matrix Mini R4.

 * Author: Anthony
 * Modified 5 Oct 2025

  www.matrixrobotics.com
  
  Calib Step:
  1. Securely attach the R4 Controller to a level and stable surface or a specialized mounting fixture.
  2. The goal is to align each of the sensor's axes (X, Y, Z) with the direction of the gravitational field.
  3. Perform rotations to achieve six distinct orientations, where each axis is pointed upwards and downwards.
  4. Record the 6 faces raw acc value.
  5. Call MiniR4.Motion.saveIMUCalData() function to save calibration data into Arduino R4 EEPROM.
  6. The calibration will be permanently saved in the Arduino R4 EEPROM (0x32 - 0x49) and automatically applied at each boot.
  
*/
#include <MatrixMiniR4.h>

void readRealAcc(){
  float accData[4];
  while(1){ 
    MiniR4.Motion.getIMU_acc_real(accData);
    Serial.print("X: ");
    Serial.print(accData[0]);
    Serial.print(", Y: ");
    Serial.print(accData[1]);
    Serial.print(", Z: ");
    Serial.println(accData[2]);
	delay(250);
  }
}

void setup() {
  MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2, two-cell (2S)
  Serial.println("\nMATRIX Mini R4 Test - IMU Accel calibration\n");
  Serial.println("Starting Up ... \n");
  Serial.println("Step: ");
  Serial.println("1. Securely attach the R4 Controller to a level and stable surface or a specialized mounting fixture.");
  Serial.println("2. The goal is to align each of the sensor's axes (X, Y, Z) with the direction of the gravitational field.");
  Serial.println("3. Perform rotations to achieve six distinct orientations, where each axis is pointed upwards and downwards.");
  Serial.println("4. Record the 6 faces raw acc value.");
  Serial.println("5. Call MiniR4.Motion.saveIMUCalData() function to save calibration data into Arduino R4 EEPROM.");
  Serial.println("6. The calibration will be permanently saved in the Arduino R4 EEPROM (0x32 - 0x49) and automatically applied at each boot.\n");

  MiniR4.Motion.resetIMUValues(); // Reset IMU

  delay(1000);
}

void loop() {
	
  //First, use this code to measure and record the X, Y, and Z values ​​of the R4 controller when placed.
  readRealAcc();
  
  //Second, after measure and record, using this function to save value into EEPROM.
  // Don't forget to change value and comment First part code, uncomment the code below to apply.
  // MiniR4.Motion.saveIMUCalData(7.66, 8.42, 7.93, 7.98, 7.39, 9.02);
  
}
