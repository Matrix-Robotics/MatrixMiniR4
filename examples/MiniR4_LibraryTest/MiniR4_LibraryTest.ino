/*
  Matrix Mini R4 Library Test

 * Lib: v1.1.3
 * FW: v3.2
 * Author: Barry
 * Modified: 10 Oct 2024

  www.matrixrobotics.com
*/
#include <MatrixMiniR4.h>

void setup() {
  bool ret = MiniR4.begin();
  Serial.begin(115200);
  MiniR4.PWR.setBattCell(2);  // 18650x2

  MiniR4.LED.setColor(1, 0, 0, 0);  // Turn off LED
  MiniR4.LED.setColor(2, 0, 0, 0);

  MiniR4.M1.setReverse(true);  // Set direction
  MiniR4.M2.setReverse(false);
  MiniR4.M3.setReverse(false);
  MiniR4.M4.setReverse(false);

  MiniR4.M1.setBrake(true);  // Motor Brake
  MiniR4.M2.setBrake(true);
  MiniR4.M3.setBrake(true);
  MiniR4.M4.setBrake(true);

  MiniR4.RC1.setHWDir(true);  // Set Servo direction
  MiniR4.RC2.setHWDir(false);
  MiniR4.RC3.setHWDir(false);
  MiniR4.RC4.setHWDir(false);

  MiniR4.RC1.setAngle(0);  // Set Servo angle
  MiniR4.RC2.setAngle(0);
  MiniR4.RC3.setAngle(0);
  MiniR4.RC4.setAngle(0);

  MiniR4.Buzzer.NoTone();  // Mute the buzzer

  if (ret) {
    Serial.println("Matrix Mini R4 init success");
  } else {
    Serial.println("Matrix Mini R4 init failed");
  }

  if (MiniR4.Motion.resetIMUValues()) {
    Serial.println("Matrix Mini R4 resetIMUValues success");
  } else {
    Serial.println("Matrix Mini R4 resetIMUValues failed");
  }

  delay(1000);
}

void loop() {
  // TaskLED();
  // TaskButton();
  // TaskOLED();
  // TaskBuzzer();
  // TaskPowerView();
  // TaskIMU();
  // TaskMotorPower();
  // TaskMotorSpeed();
  // TaskEncoder();
  // TaskServo();
  // TaskDI();
  // TaskAI();
  // TaskGrayscale(); // A1
  // TaskI2CColor();  // I2C1
  // TaskI2CMotion(); // I2C2
  // TaskI2CLaser();  // I2C3
  // TaskI2CMXCtrl(); // I2C4
  // TaskMVision(); // UART
  // TaskMJ2(); // D2(4p)  D3(2p)
  // TaskDHT11(); // D1
  // TaskDS18B20(); // D2
  // TaskGroveUS(); // D3
}

void TaskLED() {
  MiniR4.LED.setColor(1, 255, 0, 0);  // Red
  MiniR4.LED.setColor(2, 255, 0, 0);
  delay(500);
  MiniR4.LED.setColor(1, 0, 255, 0);  // Green
  MiniR4.LED.setColor(2, 0, 255, 0);
  delay(500);
  MiniR4.LED.setColor(1, 0, 0, 255);  // Blue
  MiniR4.LED.setColor(2, 0, 0, 255);
  delay(500);
}

void TaskButton() {
  bool BTN_UP = MiniR4.BTN_UP.getState();
  bool BTN_DOWN = MiniR4.BTN_DOWN.getState();

  Serial.print("BTN_UP: ");
  Serial.print(BTN_UP);
  Serial.print(", BTN_DOWN: ");
  Serial.println(BTN_DOWN);
  delay(100);
}

void TaskOLED() {
  MiniR4.OLED.clearDisplay();
  MiniR4.OLED.setTextSize(1);
  MiniR4.OLED.setTextColor(WHITE);
  MiniR4.OLED.setCursor(10, 10);
  MiniR4.OLED.print("Hello World!!");
  MiniR4.OLED.display();
  delay(100);
}

void TaskBuzzer() {
  MiniR4.Buzzer.Tone(NOTE_C4, 1000);
  delay(1000);
  MiniR4.Buzzer.NoTone();
  delay(500);
}

void TaskPowerView() {
  float voltage = MiniR4.PWR.getBattVoltage();
  float percentage = MiniR4.PWR.getBattPercentage();

  MiniR4.OLED.clearDisplay();
  MiniR4.OLED.setCursor(5, 5);
  MiniR4.OLED.print("Vol:" + String(voltage) + "V");
  MiniR4.OLED.setCursor(5, 20);
  MiniR4.OLED.print("Per:" + String(percentage) + "%");
  MiniR4.OLED.display();

  delay(100);
}

void TaskIMU() {
  int roll = MiniR4.Motion.getEuler(MiniR4Motion::AxisType::Roll);
  int pitch = MiniR4.Motion.getEuler(MiniR4Motion::AxisType::Pitch);
  int yaw = MiniR4.Motion.getEuler(MiniR4Motion::AxisType::Yaw);

  char buff[80];
  sprintf(buff, "Euler: r=%3d, p=%3d, y=%3d\t\n", roll, pitch, yaw);
  Serial.print(buff);

  delay(100);
}

void TaskMotorPower() {
  MiniR4.M1.begin();
  MiniR4.M2.begin();
  MiniR4.M3.begin();
  MiniR4.M4.begin();

  int power = 100;

  while (true) {
    MiniR4.M1.setPower(power);
    MiniR4.M2.setPower(power);
    MiniR4.M3.setPower(power);
    MiniR4.M4.setPower(power);
    delay(2000);
    power *= -1;
  }
}

void TaskMotorSpeed() {
  MiniR4.M1.begin();
  MiniR4.M2.begin();
  MiniR4.M3.begin();
  MiniR4.M4.begin();

  int speed = 100;

  while (true) {
    MiniR4.M1.setPower(speed);
    MiniR4.M2.setPower(speed);
    MiniR4.M3.setPower(speed);
    MiniR4.M4.setPower(speed);
    delay(2000);
    speed *= -1;
  }
}

void TaskEncoder() {
  MiniR4.M1.resetCounter();
  MiniR4.M2.resetCounter();
  MiniR4.M3.resetCounter();
  MiniR4.M4.resetCounter();

  while (true) {
    Serial.print("M1: ");
    Serial.print(MiniR4.M1.getDegrees());
    Serial.print(", M2: ");
    Serial.print(MiniR4.M2.getDegrees());
    Serial.print(", M3: ");
    Serial.print(MiniR4.M3.getDegrees());
    Serial.print(", M4: ");
    Serial.println(MiniR4.M4.getDegrees());

    MiniR4.M1.setSpeed(50);
    MiniR4.M2.setSpeed(50);
    MiniR4.M3.setSpeed(50);
    MiniR4.M4.setSpeed(50);
    delay(100);
  }
}

void TaskServo() {
  int i;
  for (i = 0; i < 180; i++) {
    MiniR4.RC1.setAngle(i);
    MiniR4.RC2.setAngle(i);
    MiniR4.RC3.setAngle(i);
    MiniR4.RC4.setAngle(i);
    delay(15);
  }
  for (i = 180; i > 0; i--) {
    MiniR4.RC1.setAngle(i);
    MiniR4.RC2.setAngle(i);
    MiniR4.RC3.setAngle(i);
    MiniR4.RC4.setAngle(i);
    delay(15);
  }
}

void TaskDI() {
  bool d1L = MiniR4.D1.getL();
  bool d1R = MiniR4.D1.getR();
  bool d2L = MiniR4.D2.getL();
  bool d2R = MiniR4.D2.getR();
  bool d3L = MiniR4.D3.getL();
  bool d3R = MiniR4.D3.getR();
  bool d4L = MiniR4.D4.getL();
  bool d4R = MiniR4.D4.getR();

  char buff[80];
  sprintf(buff, "D1: %d, %d, D2: %d, %d, D3: %d, %d, D4: %d, %d", d1L, d1R, d2L, d2R, d3L, d3R, d4L, d4R);
  Serial.println(buff);

  delay(100);
}

void TaskAI() {
  int A1L = MiniR4.A1.getAIL();
  int A1R = MiniR4.A1.getAIR();
  int A2L = MiniR4.A2.getAIL();
  int A2R = MiniR4.A2.getAIR();
  int A3L = MiniR4.A3.getAIL();
  int A3R = MiniR4.A3.getAIR();

  char buff[80];
  sprintf(buff, "AI1: %d, %d, AI2: %d, %d, AI3: %d, %d", A1L, A1R, A2L, A2R, A3L, A3R);
  Serial.println(buff);

  delay(100);
}

void TaskGrayscale() {
  int AI = MiniR4.A1.getAIL();  // Analog read
  int DI = MiniR4.A1.getR();    // Digital read

  char buff[50];
  sprintf(buff, "AI: %d, DI: %d", AI, DI);
  Serial.println(buff);

  delay(100);
}

void TaskI2CColor() {
  if (MiniR4.I2C1.MXColor.begin()) {
    Serial.println("MXColor init success");
  } else {
    Serial.println("MXColor init failed");
  }

  while (true) {
    int r = MiniR4.I2C1.MXColor.getColor(R);
    int g = MiniR4.I2C1.MXColor.getColor(G);
    int b = MiniR4.I2C1.MXColor.getColor(B);
    int c = MiniR4.I2C1.MXColor.getColor(C);
    int m = MiniR4.I2C1.MXColor.getColor(M);
    int y = MiniR4.I2C1.MXColor.getColor(Y);
    int k = MiniR4.I2C1.MXColor.getColor(K);

    char buff[64];
    sprintf(buff, "R: %3d, G: %3d, B: %3d, C: %3d, M: %3d, Y: %3d, K: %3d\n", r, g, b, c, m, y, k);
    Serial.print(buff);

    delay(100);
  }
}

void TaskI2CMotion() {
  if (MiniR4.I2C4.MXMotion.begin()) {
    Serial.println("MXMotion init success");
  } else {
    Serial.println("MXMotion init failed");
  }

  while (true) {
    int roll = MiniR4.I2C2.MXMotion.getRoll();
    int pitch = MiniR4.I2C2.MXMotion.getPitch();
    int yaw = MiniR4.I2C2.MXMotion.getYaw();

    Serial.print("Roll: ");
    Serial.print(roll);
    Serial.print(" , Pitch: ");
    Serial.print(pitch);
    Serial.print(" , Yaw: ");
    Serial.println(yaw);

    delay(100);
  }
}

void TaskI2CLaser() {
  if (MiniR4.I2C3.MXLaser.begin()) {
    Serial.println("MXLaser init success");
  } else {
    Serial.println("MXLaser init failed");
  }

  while (true) {
    int dis = MiniR4.I2C3.MXLaser.getDistance();

    if (dis == 8191) {
      Serial.println("TIMEOUT");
    } else {
      Serial.print("Distance = ");
      Serial.print(dis);
      Serial.println("mm");
    }
    delay(100);
  }
}

void TaskI2CMXCtrl() {
  MiniR4.I2C4.MXCtrl.motorInvert(1, false);  // Set Direction

  MiniR4.I2C4.MXCtrl.servoSet(1, 0);   // Servo Motor
  MiniR4.I2C4.MXCtrl.motorSet(1, 60);  // Large DC Motor
  delay(1000);

  MiniR4.I2C4.MXCtrl.servoSet(1, 90);
  MiniR4.I2C4.MXCtrl.motorSet(1, 0);
  delay(500);
}

void TaskMVision() {
  unsigned int CamData[10];
  int CamStatus;
  MiniR4.Vision.Begin();

  while (true) {
    CamStatus = MiniR4.Vision.SmartCamReader(CamData);
    if (CamStatus > 0) {
      Serial.println(CamData[0]);
    } else {
      Serial.println("None");
    }
    delay(100);
  }
}

void TaskMJ2() {
  MiniR4.PS2.read_gamepad(false, 0);

  int LY = MiniR4.PS2.Analog(PSS_LY);
  int RX = MiniR4.PS2.Analog(PSS_RX);

  Serial.print("LY: ");
  Serial.print(LY);
  Serial.print(", RX: ");
  Serial.println(RX);

  delay(10);
}

void TaskDHT11() {
  int temperature = MiniR4.D1.DHT11.readTemperature();
  int humidity = MiniR4.D1.DHT11.readHumidity();

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(", Humi: ");
  Serial.println(humidity);

  delay(100);
}

void TaskDS18B20() {
  int temperature = MiniR4.D2.DS18B20.requestAndGetTemp();

  Serial.print("Temp: ");
  Serial.println(temperature);

  delay(100);
}

void TaskGroveUS() {
  int dis = MiniR4.D1.GroveUS.MeasureInCentimeters();

  Serial.print("Distance: ");
  Serial.print(dis);
  Serial.println("cm");

  delay(100);
}