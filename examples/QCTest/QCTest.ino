#include "CircusButton.h"
#include <MatrixMiniR4.h>

CircusButton btnA(-1, 1);
CircusButton btnB(-1, 1);

uint8_t page0State = 0;
uint8_t page1State = 0;
uint8_t page2State = 0;
uint8_t page3State = 0;
uint8_t page4State = 0;

void setup(void)
{
    Serial.begin(115200);

    btnA.SetTKeyPolling(50000);
    btnB.SetTKeyPolling(50000);
    btnA.SetTKeyRepeatStart(1000000);
    btnB.SetTKeyRepeatStart(1000000);
    btnA.SetTKeyRepeatWork(300000);
    btnB.SetTKeyRepeatWork(300000);

    bool ret = MiniR4.begin();
    if (ret) {
        Serial.println("Matrix Mini R4 init success");
    } else {
        Serial.println("Matrix Mini R4 init failed");
    }
    Stop();
}

void loop(void)
{
    static uint8_t state = 0;

    bool btnAFlag = MiniR4.BTN_UP.getState();
    bool btnBFlag = MiniR4.BTN_DOWN.getState();
    btnA.loopEx(btnAFlag);
    btnB.loopEx(btnBFlag);

    if (btnB.readState() == BTN_STATE::F_EDGE) {
        Stop();
        page0State = 0;
        page1State = 0;
        page2State = 0;
        page3State = 0;
        page4State = 0;
        state      = (state + 1) % 5;
        MiniR4.Buzzer.Tone(500, 50);
    }

    switch (state) {
    case 0: TaskPage0(); break;
    case 1: TaskPage1(); break;
    case 2: TaskPage2(); break;
    case 3: TaskPage3(); break;
    case 4: TaskPage4(); break;
    default: break;
    }
}
// Test Button、OLED、STM ADC、Info
void TaskPage0(void)
{
    static uint32_t oledTimer = 0;

    switch (page0State) {
    case 0:   // init
    {
        MiniR4.PWR.setBattCell(2);
        page0State = 1;
    } break;
    case 1:   // Run
    {
        if (millis() >= oledTimer) {
            oledTimer = millis() + 150;
            MiniR4.OLED.clearDisplay();
            MiniR4.OLED.setTextColor(WHITE);
            float volt = MiniR4.PWR.getBattVoltage();
            float perc = MiniR4.PWR.getBattPercentage();
            MiniR4.OLED.setCursor(0, 0);
            MiniR4.OLED.print("Batt  Voltage:");
            MiniR4.OLED.print(volt, 2);
            MiniR4.OLED.print("V");
            MiniR4.OLED.setCursor(0, 16);
            MiniR4.OLED.print("Batt  Percent:");
            MiniR4.OLED.print(perc, 2);
            MiniR4.OLED.print("%");
            MiniR4.OLED.display();
        }
    } break;
    default: break;
    }
}
// Test Button、OLED、Motor、Encoder、Servo
void TaskPage1(void)
{
    static bool     dir       = true;
    static uint32_t oledTimer = 0;

    switch (page1State) {
    case 0:   // init
    {
        dir = true;
        MiniR4.M1.setReverse(true);
        MiniR4.M2.setReverse(true);
        MiniR4.M3.setReverse(true);
        MiniR4.M4.setReverse(true);
        MiniR4.RC1.setHWDir(true);
        MiniR4.RC2.setHWDir(true);
        MiniR4.RC3.setHWDir(true);
        MiniR4.RC4.setHWDir(true);

        MiniR4.M1.setPower(100);
        MiniR4.M2.setPower(100);
        MiniR4.M3.setPower(100);
        MiniR4.M4.setPower(100);

        MiniR4.RC1.setAngle(180);
        MiniR4.RC2.setAngle(180);
        MiniR4.RC3.setAngle(180);
        MiniR4.RC4.setAngle(180);

        page1State = 1;
    } break;
    case 1:   // Run
    {
        if (btnA.readState() == BTN_STATE::F_EDGE) {
            dir           = !dir;
            int16_t speed = dir ? 100 : -100;
            MiniR4.M1.setPower(speed);
            MiniR4.M2.setPower(speed);
            MiniR4.M3.setPower(speed);
            MiniR4.M4.setPower(speed);
            MiniR4.RC1.setHWDir(dir);
            MiniR4.RC2.setHWDir(dir);
            MiniR4.RC3.setHWDir(dir);
            MiniR4.RC4.setHWDir(dir);
            MiniR4.RC1.setAngle(180);
            MiniR4.RC2.setAngle(180);
            MiniR4.RC3.setAngle(180);
            MiniR4.RC4.setAngle(180);
            MiniR4.Buzzer.Tone(700, 50);
        }

        if (millis() >= oledTimer) {
            oledTimer = millis() + 150;
            MiniR4.OLED.clearDisplay();
            MiniR4.OLED.setTextColor(WHITE);
            MiniR4.OLED.setCursor(0, 0);
            MiniR4.OLED.print("M1:");
            MiniR4.OLED.print(MiniR4.M1.getCounter());
            MiniR4.OLED.setCursor(0, 16);
            MiniR4.OLED.print("M2:");
            MiniR4.OLED.print(MiniR4.M2.getCounter());
            MiniR4.OLED.setCursor(64, 0);
            MiniR4.OLED.print("M3:");
            MiniR4.OLED.print(MiniR4.M3.getCounter());
            MiniR4.OLED.setCursor(64, 16);
            MiniR4.OLED.print("M4:");
            MiniR4.OLED.print(MiniR4.M4.getCounter());
            MiniR4.OLED.display();
        }
    } break;
    default: break;
    }
}
// Test Button、OLED、IMU、LED、Buzzer
void TaskPage2(void)
{
    static uint32_t oledTimer      = 0;
    static uint32_t colorArr[]     = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFFFF, 0x000000};
    static uint8_t  colorIdx       = 0;
    static uint8_t  brightnessMode = 4;

    switch (page2State) {
    case 0:   // init
    {
        colorIdx       = 0;
        brightnessMode = 4;
        page2State     = 1;
    } break;
    case 1:   // Run
    {
        if (btnA.readState() == BTN_STATE::REPEAT) {
            brightnessMode++;
            brightnessMode &= 0x07;
            MiniR4.LED.setBrightness(1, 0xFF >> brightnessMode);
            MiniR4.LED.setBrightness(2, 0xFF >> brightnessMode);
            MiniR4.Buzzer.Tone(1000, 30);
        } else if (btnA.readState() == BTN_STATE::R_EDGE) {
            colorIdx = (colorIdx + 1) % 5;
            MiniR4.LED.setColor(1, colorArr[colorIdx]);
            MiniR4.LED.setColor(2, colorArr[colorIdx]);
            MiniR4.Buzzer.Tone(700, 50);
        }

        if (millis() >= oledTimer) {
            oledTimer = millis() + 150;
            MiniR4.OLED.clearDisplay();
            MiniR4.OLED.setTextColor(WHITE);
            MiniR4.OLED.setCursor(0, 0);
            MiniR4.OLED.print("AX:");
            MiniR4.OLED.print(String(MiniR4.Motion.getAccel(MiniR4Motion::AxisType::X), 3));
            MiniR4.OLED.setCursor(0, 12);
            MiniR4.OLED.print("AY:");
            MiniR4.OLED.print(String(MiniR4.Motion.getAccel(MiniR4Motion::AxisType::Y), 3));
            MiniR4.OLED.setCursor(0, 24);
            MiniR4.OLED.print("AZ:");
            MiniR4.OLED.print(String(MiniR4.Motion.getAccel(MiniR4Motion::AxisType::Z), 3));

            MiniR4.OLED.setCursor(64, 0);
            MiniR4.OLED.print("GX:");
            MiniR4.OLED.print(String(MiniR4.Motion.getGyro(MiniR4Motion::AxisType::X), 3));
            MiniR4.OLED.setCursor(64, 12);
            MiniR4.OLED.print("GY:");
            MiniR4.OLED.print(String(MiniR4.Motion.getGyro(MiniR4Motion::AxisType::Y), 3));
            MiniR4.OLED.setCursor(64, 24);
            MiniR4.OLED.print("GZ:");
            MiniR4.OLED.print(String(MiniR4.Motion.getGyro(MiniR4Motion::AxisType::Z), 3));
            MiniR4.OLED.display();
        }
    } break;
    default: break;
    }
}
// Test Button、OLED、I2C Mux
void TaskPage3(void)
{
    static uint32_t oledTimer = 0;

    switch (page3State) {
    case 0:   // init
    {
        page3State = 1;
    } break;
    case 1:   // Run
    {
        uint8_t addrArr[4] = {0xFF, 0xFF, 0xFF, 0xFF};
        for (uint8_t t = 0; t < 4; t++) {
            Wire1.beginTransmission(0x70);
            Wire1.write(1 << t);
            Wire1.endTransmission();
            for (uint8_t addr = 0; addr <= 127; addr++) {
                if (addr == 0x70 || addr == 0x3D) continue;
                Wire1.beginTransmission(addr);
                if (!Wire1.endTransmission()) {
                    addrArr[t] = addr;
                }
            }
        }
        if (millis() >= oledTimer) {
            oledTimer = millis() + 150;
            MiniR4.OLED.clearDisplay();
            MiniR4.OLED.setTextColor(WHITE);
            MiniR4.OLED.setCursor(0, 0);
            MiniR4.OLED.print("I2C1:0x");
            MiniR4.OLED.print(addrArr[0], HEX);
            MiniR4.OLED.setCursor(0, 16);
            MiniR4.OLED.print("I2C2:0x");
            MiniR4.OLED.print(addrArr[1], HEX);
            MiniR4.OLED.setCursor(64, 0);
            MiniR4.OLED.print("I2C3:0x");
            MiniR4.OLED.print(addrArr[2], HEX);
            MiniR4.OLED.setCursor(64, 16);
            MiniR4.OLED.print("I2C4:0x");
            MiniR4.OLED.print(addrArr[3], HEX);
            MiniR4.OLED.display();
        }
    } break;
    default: break;
    }
}
// Test Button、OLED、IO
void TaskPage4(void)
{
    static uint32_t oledTimer = 0;

    switch (page4State) {
    case 0:   // init
    {
        page4State = 1;
    } break;
    case 1:   // Run
    {
        if (millis() >= oledTimer) {
            oledTimer = millis() + 150;
            MiniR4.OLED.clearDisplay();
            MiniR4.OLED.setTextColor(WHITE);
            MiniR4.OLED.setCursor(0, 0);
            MiniR4.OLED.print("D1:");
            MiniR4.OLED.print(MiniR4.D1.getL(true));
            MiniR4.OLED.print(" , ");
            MiniR4.OLED.print(MiniR4.D1.getR(true));
            MiniR4.OLED.setCursor(0, 8);
            MiniR4.OLED.print("D2:");
            MiniR4.OLED.print(MiniR4.D2.getL(true));
            MiniR4.OLED.print(" , ");
            MiniR4.OLED.print(MiniR4.D2.getR(true));
            MiniR4.OLED.setCursor(0, 16);
            MiniR4.OLED.print("D3:");
            MiniR4.OLED.print(MiniR4.D3.getL(true));
            MiniR4.OLED.print(" , ");
            MiniR4.OLED.print(MiniR4.D3.getR(true));
            MiniR4.OLED.setCursor(0, 24);
            MiniR4.OLED.print("D4:");
            MiniR4.OLED.print(MiniR4.D4.getL(true));
            MiniR4.OLED.print(" , ");
            MiniR4.OLED.print(MiniR4.D4.getR(true));
            MiniR4.OLED.setCursor(64, 0);
            MiniR4.OLED.print("A1:");
            MiniR4.OLED.print(MiniR4.A1.getL(true));
            MiniR4.OLED.print(" , ");
            MiniR4.OLED.print(MiniR4.A1.getR(true));
            MiniR4.OLED.setCursor(64, 8);
            MiniR4.OLED.print("A2:");
            MiniR4.OLED.print(MiniR4.A2.getL(true));
            MiniR4.OLED.print(" , ");
            MiniR4.OLED.print(MiniR4.A2.getR(true));
            MiniR4.OLED.setCursor(64, 16);
            MiniR4.OLED.print("A3:");
            MiniR4.OLED.print(MiniR4.A3.getL(true));
            MiniR4.OLED.print(" , ");
            MiniR4.OLED.print(MiniR4.A3.getR(true));
            MiniR4.OLED.display();
        }
    } break;
    default: break;
    }
}

void Stop(void)
{
    MiniR4.M1.setPower(0);
    MiniR4.M2.setPower(0);
    MiniR4.M3.setPower(0);
    MiniR4.M4.setPower(0);
    MiniR4.LED.setColor(1, 0x00);
    MiniR4.LED.setColor(2, 0x00);
    MiniR4.OLED.clearDisplay();
    MiniR4.OLED.display();
}
