/**
 * @file MatrixMiniR4.cpp
 * @brief Implementation of the MatrixMiniR4 class for initializing and controlling hardware modules.
 * 
 * This file contains the implementation of the MatrixMiniR4 class, which manages various hardware components
 * such as motors, servos, LEDs, and displays. The `begin` method initializes these components, configures the 
 * OLED display, and sets up the MJ2 gamepad interface. If the initialization fails, it displays an error message 
 * on the OLED and sounds a buzzer.
 * 
 * @note The initialization sequence includes waiting for motors and servos controller to be ready and displaying an error
 * message if the MMLower initialization fails.
 * 
 * @see MatrixMiniR4.h
 */
#include "MatrixMiniR4.h"
#include "Modules/MMLower.h"

MatrixMiniR4::MatrixMiniR4() {}

/**
 * @brief Initialize the MatrixMiniR4 system and its components.
 * 
 * This function initializes the MMLower (STM32) system, sets up the RGB LED and buzzer, 
 * and pre init for all DC motors and servos =. It also configures the 
 * OLED display and the MJ2 gamepad.
 * 
 * @return true if initialization was successful, false otherwise.
 */
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

MatrixMiniR4 MiniR4; ///< The MiniR4 Main Object.
