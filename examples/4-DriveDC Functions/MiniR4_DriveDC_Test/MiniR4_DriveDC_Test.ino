/*
  Matrix Mini R4 DriveDC Example
 * Description: Demonstrates how to using DriveDC class with your R4 robot.

 * Author: Anthony
 * Modified 13 Sep 2025

  www.matrixrobotics.com
*/
#include "MatrixMiniR4.h"

// Test parameter configuration
const int testPairs[][2] = {
  { 50, 50 },
  { -50, -50 },
  { 50, 25 },
  { -50, -25 },
  { 50, -50 }
};
const int testCount = sizeof(testPairs) / sizeof(testPairs[0]);

const int turnAngles[] = { 90, -90, 270, -270 };
const int turnCount = sizeof(turnAngles) / sizeof(turnAngles[0]);

// General utility functions
void waitForButton(const String& label) {
  Serial.println();
  Serial.println("Ready: " + label);
  Serial.println("Press BTN_UP to continue...");
  while (!MiniR4.BTN_UP.getState()) {
    delay(10); // Reduce CPU usage
  }
  delay(300); // Debounce protection
}

void printStatus(const String& label) {
  Serial.print(label);
  Serial.print(" | L: ");
  Serial.print(MiniR4.M2.getDegrees());
  Serial.print(", R: ");
  Serial.print(MiniR4.M3.getDegrees());
  Serial.print(", DBASE: ");
  Serial.println(MiniR4.DriveDC.getDegrees());  
  delay(100); // Reduce delay
}

void stopMotors() {
  MiniR4.DriveDC.Move(0, 0);
}

// Motor configuration functions
void setupLEGOMotors() {
  Serial.println("Setup: MATRIX LEGO Motors");
  MiniR4.M2.setPPR_RPM(360, 300);
  MiniR4.M3.setPPR_RPM(360, 300);
  MiniR4.M2.setReverse(true);
  MiniR4.M3.setReverse(false);
  MiniR4.DriveDC.begin(2, 3, true, false);
  MiniR4.DriveDC.setMoveSyncPID(0.002, 0.00, 0.004);
  MiniR4.DriveDC.setMoveGyroPID(0.52, 0.00, 0.15);
  MiniR4.DriveDC.setTurnGyroPID(22.75, 0.02, 1.54);
}

void setupTTMotors() {
  Serial.println("Setup: Blue TT Motors");
  MiniR4.M2.setPPR_RPM(545, 200);
  MiniR4.M3.setPPR_RPM(545, 200);
  MiniR4.M2.setReverse(false);
  MiniR4.M3.setReverse(true);
  MiniR4.DriveDC.begin(2, 3, false, true);
  MiniR4.DriveDC.setMoveSyncPID(0.02, 0.00, 0.04);
  MiniR4.DriveDC.setMoveGyroPID(6.01, 0.00, 2.15);
  MiniR4.DriveDC.setTurnGyroPID(22.75, 0.08, 0.25);
}

// Test functions
void testMotorDirection() {
  Serial.println("\n=== Motor Direction Check ===");
  
  waitForButton("Motor PWR Check - Verify motors rotate toward front direction");
  MiniR4.M2.setPower(100);
  MiniR4.M3.setPower(100);
  delay(800);
  stopMotors();

  waitForButton("DriveDC Forward Check - Verify robot moves forward");
  MiniR4.DriveDC.Move(30, 30);  
  delay(500);
  stopMotors();

  waitForButton("DriveDC Turn Check - Verify robot turns right");
  MiniR4.DriveDC.Move(30, 0);  
  delay(500);
  stopMotors();
}

void testStraightMovement() {
  Serial.println("\n=== IMU Straight Movement Test ===");
  
  waitForButton("Test 1: GyroDegs (Straight 1000 degrees)");
  MiniR4.DriveDC.MoveGyroDegs(50, 0, 1000, true);  
  printStatus("Test 1");

  waitForButton("Test 2: GyroTime (Straight 1 second)");
  MiniR4.DriveDC.MoveGyroTime(60, 0, 1, true);
  printStatus("Test 2");
}

void testTurning() {
  Serial.println("\n=== IMU Turning Test ===");
  
  // Single wheel turning test
  for (int i = 0; i < turnCount; i++) {
    int angle = turnAngles[i];
    waitForButton("Single Wheel Turn " + String(angle) + " degrees");
    MiniR4.DriveDC.TurnGyro(50, angle, 0, true);  
    delay(100);
    printStatus("TurnGyro1 " + String(MiniR4.Motion.getEuler(MiniR4Motion::AxisType::Yaw)));
  }

  // Dual wheel turning test
  for (int i = 0; i < turnCount; i++) {
    int angle = turnAngles[i];
    waitForButton("Dual Wheel Turn " + String(angle) + " degrees");
    MiniR4.DriveDC.TurnGyro(40, angle, 1, true);  
    printStatus("TurnGyro2 " + String(MiniR4.Motion.getEuler(MiniR4Motion::AxisType::Yaw)));
  }

  // Small angle test
  waitForButton("Small Angle Test: Dual Wheel Turn 45 degrees");
  MiniR4.DriveDC.TurnGyro(80, 45, 1, true);  
}

void testPowerCombinations() {
  Serial.println("\n=== Power Combination Test ===");
  
  for (int i = 0; i < testCount; i++) {
    int left = testPairs[i][0];
    int right = testPairs[i][1];
    String label = "Power (" + String(left) + ", " + String(right) + ")";

    // Synchronized movement test
    MiniR4.DriveDC.resetCounter();  
    waitForButton("MoveSyncDegs " + label);  
    MiniR4.DriveDC.MoveSyncDegs(left, right, 600, true);  
    printStatus("SyncDegs " + label);

    // Asynchronous movement test
    MiniR4.DriveDC.resetCounter();  
    waitForButton("MoveDegs " + label);  
    MiniR4.DriveDC.MoveDegs(left, right, 600, true);  
    printStatus("NoSyncDegs " + label);

    Serial.println("------");
  }
}

void testAdvancedMovement() {
  Serial.println("\n=== Advanced Movement Test ===");
  
  waitForButton("Synchronized Straight Movement (2000 degrees)");
  MiniR4.DriveDC.MoveSyncDegs(50, 50, 2000, true);

  waitForButton("Manual Control Synchronized Movement (2000 degrees)");
  MiniR4.DriveDC.resetCounter();  
  while (MiniR4.DriveDC.getDegrees() < 2000) {  
    MiniR4.DriveDC.MoveSync(50, 50);  
    delay(10); // Avoid excessive CPU usage
  }
  stopMotors();
}

void setup() {
  MiniR4.begin();
  MiniR4.PWR.setBattCell(2);
  Serial.begin(9600);

  Serial.println("===== MATRIX DriveDC Comprehensive Test Start =====");

  // Select motor configuration (uncomment one of them)
  // setupLEGOMotors(); //Left M2, Right M3
  setupTTMotors(); //Left M2, Right M3

  // Execute all tests
  testMotorDirection();
  testStraightMovement();
  testTurning();
  testPowerCombinations();
  testAdvancedMovement();

  Serial.println("\n===== All Tests Completed =====");
}

void loop() {
  // test complete
  delay(1);
}