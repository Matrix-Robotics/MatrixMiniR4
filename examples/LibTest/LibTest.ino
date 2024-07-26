#include <MatrixMiniR4.h>

void setup(void)
{
    Serial.begin(115200);

    bool ret = MiniR4.begin();
    MiniR4.M1.setReverse(true);
    MiniR4.M2.setReverse(false);
    MiniR4.M3.setReverse(false);
    MiniR4.M4.setReverse(false);

    MiniR4.RC1.setHWDir(true);
    MiniR4.RC2.setHWDir(true);
    MiniR4.RC3.setHWDir(true);
    MiniR4.RC4.setHWDir(true);

    MiniR4.LED.setBrightness(1, 100);
    MiniR4.LED.setBrightness(2, 100);

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
}

void loop(void)
{
    // TaskLED();           // pass
    // TaskButton();        // pass
    // TaskMotor();         // pass
    // TaskServo();         // pass
    // TaskMotion();        // pass
    // TaskBuzzer();        // pass
    // TaskEncoder();       // pass
    // TaskOLED();          // pass
    // TaskWiFi();          // pass
    // TaskI2CMotion();     // pass
    // TaskI2CLaser();      // pass
    // TaskDIO();           // pass
    // TaskAIO();           // pass
    // TaskGrayScale();     // pass
    // TaskPS2();           // pass
    // TaskI2CColor();      // pass
    // TaskPower();         // pass
    TaskMotorRotationDeg();   // pass

    // TaskVernier();
    // TaskVision();
}

void TaskLED(void)
{
    static uint32_t timer = 0;
    static uint8_t  idx   = 0;

    uint32_t colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFFFF, 0x000000};
    if (millis() >= timer) {
        timer = millis() + 500;
        MiniR4.LED.setColor(1, 0xFF, 0xFF, 0x00);
        MiniR4.LED.setColor(2, colors[idx]);
        if (++idx >= 5) idx = 0;
    }
}

void TaskMotor(void)
{
    static uint32_t timer = 0;
    static int8_t   speed = 100;

    if (millis() >= timer) {
        timer = millis() + 5000;
        speed *= -1;
        MiniR4.M1.setPower(speed);
        MiniR4.M2.setPower(speed);
        MiniR4.M3.setPower(speed);
        MiniR4.M4.setPower(speed);
    }
}

void TaskServo(void)
{
    static uint32_t timer = 0;
    static uint8_t  angle = 0;

    if (millis() >= timer) {
        timer = millis() + 50;
        angle += 5;
        MiniR4.RC1.setAngle(angle);
        MiniR4.RC2.setAngle(angle);
        MiniR4.RC3.setAngle(angle);
        MiniR4.RC4.setAngle(angle);
        if (angle > 180) angle = 0;
    }
}

void TaskButton(void)
{
    static uint32_t timer = 0;

    if (millis() >= timer) {
        timer     = millis() + 200;
        bool btnA = MiniR4.BTN_UP.getState();
        bool btnB = MiniR4.BTN_DOWN.getState();

        Serial.print("BTN_UP: ");
        Serial.print(btnA);
        Serial.print(" , BTN_DOWN: ");
        Serial.println(btnB);
    }
}

void TaskMotion(void)
{
    static uint32_t timer = 0;

    if (millis() >= timer) {
        timer         = millis() + 200;
        double  ax    = MiniR4.Motion.getAccel(MiniR4Motion::AxisType::X);
        double  ay    = MiniR4.Motion.getAccel(MiniR4Motion::AxisType::Y);
        double  az    = MiniR4.Motion.getAccel(MiniR4Motion::AxisType::Z);
        int16_t gx    = (int16_t)MiniR4.Motion.getGyro(MiniR4Motion::AxisType::X);
        int16_t gy    = (int16_t)MiniR4.Motion.getGyro(MiniR4Motion::AxisType::Y);
        int16_t gz    = (int16_t)MiniR4.Motion.getGyro(MiniR4Motion::AxisType::Z);
        int16_t roll  = MiniR4.Motion.getEuler(MiniR4Motion::AxisType::Roll);
        int16_t pitch = MiniR4.Motion.getEuler(MiniR4Motion::AxisType::Pitch);
        int16_t yaw   = MiniR4.Motion.getEuler(MiniR4Motion::AxisType::Yaw);

        char buff[128];
        sprintf(
            buff,
            "Accel: x=%5.2f, y=%5.2f, z=%5.2f\tG\n"
            "Gyro : x=%5d, y=%5d, z=%5d\t°/s\n"
            "Euler: r=%5d, p=%5d, y=%5d\t°\n\n",
            ax,
            ay,
            az,
            gx,
            gy,
            gz,
            roll,
            pitch,
            yaw);
        Serial.print(buff);
    }
}

void TaskEncoder(void)
{
    static uint32_t timer = 0;

    if (millis() >= timer) {
        timer        = millis() + 200;
        int32_t enc1 = MiniR4.M1.getCounter();
        int32_t enc2 = MiniR4.M2.getCounter();
        int32_t enc3 = MiniR4.M3.getCounter();
        int32_t enc4 = MiniR4.M4.getCounter();

        char buff[100];
        sprintf(buff, "Encoder: %ld, %ld, %ld, %ld", enc1, enc2, enc3, enc4);

        Serial.println(buff);
    }
}

void TaskBuzzer(void)
{
    static uint32_t timer = 0;
    static uint8_t  idx   = 0;

    uint16_t notes[] = {262, 294, 330, 349, 392, 440, 494, 523};

    if (millis() >= timer) {
        timer = millis() + 150;
        MiniR4.Buzzer.Tone(notes[idx], 100);
        if (++idx >= 8) idx = 0;
    }
}

void TaskOLED(void)
{
    static uint32_t timer = 0;

    if (millis() >= timer) {
        timer = millis() + 1000;

        MiniR4.OLED.clearDisplay();
        MiniR4.OLED.setTextSize(3);
        MiniR4.OLED.setTextColor(SSD1306_WHITE);
        MiniR4.OLED.setCursor(10, 10);
        MiniR4.OLED.print(String(millis() / 1000) + "s");
        MiniR4.OLED.display();
    }
}

void TaskWiFi(void)
{
    static uint32_t timer = 0;

    if (millis() >= timer) {
        timer = millis() + 5000;

        Serial.println("** Scan Networks **");
        int numSsid = MiniR4.WiFi.scanNetworks();
        if (numSsid == -1) {
            Serial.println("Couldn't get a WiFi connection");
            while (true);
        }

        Serial.print("number of available networks:");
        Serial.println(numSsid);

        for (int thisNet = 0; thisNet < numSsid; thisNet++) {
            Serial.print(thisNet);
            Serial.print(") ");
            Serial.print(MiniR4.WiFi.SSID(thisNet));
            Serial.print(" Signal: ");
            Serial.print(MiniR4.WiFi.RSSI(thisNet));
            Serial.println(" dBm");
        }
    }
}

void TaskI2CMotion(void)
{
    static bool initFlag = false;

    if (!initFlag) {
        bool ret = MiniR4.I2C4.MXMotion.begin();

        if (ret) {
            Serial.println("MXMotion init success");
        } else {
            Serial.println("MXMotion init failed");
        }
        initFlag = true;
    }

    static uint32_t timer = 0;
    if (millis() >= timer) {
        timer         = millis() + 200;
        int16_t roll  = MiniR4.I2C4.MXMotion.getRoll();
        int16_t pitch = MiniR4.I2C4.MXMotion.getPitch();
        int16_t yaw   = MiniR4.I2C4.MXMotion.getYaw();

        Serial.print("Roll: ");
        Serial.print(roll);
        Serial.print(" , Pitch: ");
        Serial.print(pitch);
        Serial.print(" , Yaw: ");
        Serial.println(yaw);
    }
}

void TaskI2CColor(void)
{
    static bool initFlag = false;

    if (!initFlag) {
        bool ret = MiniR4.I2C1.MXColor.begin();

        if (ret) {
            Serial.println("MXColor init success");
        } else {
            Serial.println("MXColor init failed");
        }
        initFlag = true;
    }

    static uint32_t timer = 0;
    if (initFlag) {
        if (millis() >= timer) {
            timer     = millis() + 500;
            uint8_t r = MiniR4.I2C1.MXColor.getColor(R);
            uint8_t g = MiniR4.I2C1.MXColor.getColor(G);
            uint8_t b = MiniR4.I2C1.MXColor.getColor(B);
            uint8_t c = MiniR4.I2C1.MXColor.getColor(C);
            uint8_t m = MiniR4.I2C1.MXColor.getColor(M);
            uint8_t y = MiniR4.I2C1.MXColor.getColor(Y);
            uint8_t k = MiniR4.I2C1.MXColor.getColor(K);

            char buff[64];
            sprintf(
                buff,
                "R: %3d, G: %3d, B: %3d, C: %3d, M: %3d, Y: %3d, K: %3d\n",
                r,
                g,
                b,
                c,
                m,
                y,
                k);
            Serial.print(buff);
        }
    }
}

void TaskI2CLaser(void)
{
    static bool initFlag = false;

    if (!initFlag) {
        bool ret = MiniR4.I2C0.MXLaser.begin();

        if (ret) {
            Serial.println("MXLaser init success");
        } else {
            Serial.println("MXLaser init failed");
        }
        initFlag = true;
    }

    static uint32_t timer = 0;
    if (millis() >= timer) {
        timer   = millis() + 100;
        int dis = MiniR4.I2C0.MXLaser.getDistance();

        if (dis == 8191) {
            Serial.println("TIMEOUT");
        } else {
            Serial.print("Distance = ");
            Serial.print(dis);
            Serial.println("mm");
        }
    }
}

void TaskDIO(void)
{
    static uint32_t timer = 0;
    static bool     dir   = true;
    static uint8_t  val   = 0;

    if (millis() >= timer) {
        timer = millis() + 200;
        dir   = !dir;
        val += 5;
        bool d1L = MiniR4.D1.getL();
        bool d1R = MiniR4.D1.getR();

        float distance = MiniR4.A1.US.getDistance();

        MiniR4.D3.setL(dir);
        MiniR4.D3.setR(!dir);

        bool d4L = MiniR4.D4.getL();
        bool d4R = MiniR4.D4.getR();

        char buff[50];
        sprintf(buff, "D2: %d, %d, D4: %d, %d, Distance = %d", d1L, d1R, d4L, d4R, (int)distance);
        Serial.println(buff);
    }
}

void TaskAIO(void)
{
    static uint32_t timer = 0;

    if (millis() >= timer) {
        timer = millis() + 200;

        int AI1L = MiniR4.A1.getAIL();
        int AI1R = MiniR4.A1.getAIR();

        int AI2L = MiniR4.A2.getAIL();
        int AI2R = MiniR4.A2.getAIR();

        int AI3L = MiniR4.A3.getAIL();
        int AI3R = MiniR4.A3.getAIR();

        char buff[80];
        sprintf(buff, "AI1: %d, %d, AI2: %d, %d, AI3: %d, %d", AI1L, AI1R, AI2L, AI2R, AI3L, AI3R);
        Serial.println(buff);
    }
}

void TaskGrayScale(void)
{
    static uint32_t timer = 0;

    if (millis() >= timer) {
        timer = millis() + 200;

        int AI = MiniR4.A1.getAIL();   // Analog read
        int DI = MiniR4.A1.getR();     // Digital read

        char buff[50];
        sprintf(buff, "AI: %d, DI: %d", AI, DI);
        Serial.println(buff);
    }
}

void TaskPS2(void)
{
    static uint32_t timer = 0;
    if (millis() >= timer) {
        timer = millis() + 50;

        MiniR4.PS2.read_gamepad(false, 0);
        bool L1 = MiniR4.PS2.Button(PSB_L1);
        bool L2 = MiniR4.PS2.Button(PSB_L2);
        bool R1 = MiniR4.PS2.Button(PSB_R1);
        bool R2 = MiniR4.PS2.Button(PSB_R2);

        char buff[50];
        sprintf(buff, "L1: %d, L2: %d, R1: %d, R2: %d", L1, L2, R1, R2);
        Serial.println(buff);
    }
}

void TaskPower(void)
{
    static bool initFlag = false;

    if (!initFlag) {
        bool ret = MiniR4.PWR.setBattCell(3);

        if (ret) {
            Serial.println("Set battery cell success");
        } else {
            Serial.println("Set battery cell failed");
        }
        initFlag = true;
    }

    static uint32_t timer = 0;
    if (millis() >= timer) {
        timer            = millis() + 500;
        float voltage    = MiniR4.PWR.getBattVoltage();
        float percentage = MiniR4.PWR.getBattPercentage();
        Serial.print("Voltage: ");
        Serial.print(String(voltage, 2));
        Serial.print("V, Percentage: ");
        Serial.print(String(percentage, 2));
        Serial.println("%");
    }
}

void TaskVernier(void)
{
    static bool initFlag = false;

    if (!initFlag) {
        MiniR4.Vernier.autoID();
        initFlag = true;
    }

    static uint32_t timer = 0;
    if (millis() >= timer) {
        timer               = millis() + 500;
        float sensorReading = MiniR4.Vernier.readSensor();
        Serial.print(sensorReading);
        Serial.print(" ");
        Serial.println(MiniR4.Vernier.sensorUnits());
    }
}

void TaskVision(void)
{
    static bool initFlag = false;

    if (!initFlag) {
        MiniR4.Vision.Begin();
        initFlag = true;
    }

    static uint32_t timer = 0;
    if (millis() >= timer) {
        timer = millis() + 100;

        unsigned int data[20];
        int          result = MiniR4.Vision.SmartCamReader(data);
    }
}

void TaskMotorRotationDeg(void)
{
    if (Serial.available()) {
        String str = Serial.readStringUntil('\n');

        uint16_t deg = atoi(str.c_str());
        MiniR4.M1.rotateFor(75, deg);
        MiniR4.M2.rotateFor(75, deg);

        Serial.print("Degrees = ");
        Serial.println(deg);
    }
}