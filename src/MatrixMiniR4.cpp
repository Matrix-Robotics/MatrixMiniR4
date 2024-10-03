#include "MatrixMiniR4.h"
#include "Modules/MMLower.h"

MatrixMiniR4::MatrixMiniR4() {}

bool MatrixMiniR4::begin()
{
    MMLower::RESULT result = mmL.Init();

    LED.begin(7);
    Buzzer.begin(6);

    while (!M1.begin());
    while (!M2.begin());
    while (!M3.begin());
    while (!M4.begin());

    RC1.begin();
    RC2.begin();
    RC3.begin();
    RC4.begin();

    OLED = Adafruit_SSD1306(128, 32, &Wire1, -1);
    OLED.begin(SSD1306_SWITCHCAPVCC, MATRIXMINIR4_OLED_ADDRESS);
    OLED.setTextColor(SSD1306_WHITE); //Default Color White
    OLED.clearDisplay();
    OLED.display();

    /* CLK: D3R(11) , CMD: D2R(4) , SET: D3L(12) , DAT: D2L(5) */
    PS2.config_gamepad(11, 4, 12, 5, false, false);

    if (result != MMLower::RESULT::OK) {
        OLED.setCursor(4, 8);
        OLED.setTextSize(2);
        OLED.setTextColor(WHITE);
        OLED.print(F("Init Error"));
        OLED.display();
        while (true) {
            for (uint8_t i = 0; i < 3; i++) {
                Buzzer.Tone(700, 100);
                delay(100);
                Buzzer.NoTone();
                delay(100);
            }
            delay(3000);
        }
    }
    return true;
}

MatrixMiniR4 MiniR4;
