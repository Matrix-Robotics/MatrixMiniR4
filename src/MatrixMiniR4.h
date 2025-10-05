/**
 * @file MatrixMiniR4.h
 * @brief Header file for the MatrixMiniR4 system, integrating various hardware modules such as motors, sensors, I2C communication, OLED display, and more.
 * 
 * The MatrixMiniR4 system provides a framework for controlling and interacting with multiple components, 
 * such as DC motors, servos, LEDs, buttons, motion sensors, WiFi, and more, through modular classes.
 * This header file declares the primary class for interfacing with these components, offering easy-to-use methods 
 * for working with hardware modules on the Matrix Mini R4 platform.
 * 
 * @author MATRIX Robotics
 */
#ifndef MATRIXMINIR4_H
#define MATRIXMINIR4_H

#include "Modules/MiniR4Analog.h"
#include "Modules/MiniR4BTN.h"
#include "Modules/MiniR4Buzzer.h"
#include "Modules/MiniR4DAC.h"
#include "Modules/MiniR4DC.h"
#include "Modules/MiniR4DriveDC.h"
#include "Modules/MiniR4I2C.h"
#include "Modules/MiniR4LED.h"
#include "Modules/MiniR4Motion.h"
#include "Modules/MiniR4OLED.h"
#include "Modules/MiniR4PWM.h"
#include "Modules/MiniR4Power.h"
#include "Modules/MiniR4RC.h"
#include "Modules/MiniR4Tone.h"

#include "Modules/Sensors/MiniR4PS2X_lib.h"
#include "Modules/Sensors/MiniR4SmartCamReader.h"
#include "Modules/Sensors/MiniR4VernierLib.h"

#include <Arduino.h>
#include <WiFiS3.h>
#include <Wire.h>

#define MATRIXMINIR4_OLED_ADDRESS 0x3D

/**
 * @brief Main class for the MatrixMiniR4 system.
 *
 * @note You can using MiniR4 object to control all functionality.
 *
 * The MatrixMiniR4 class integrates various modules such as DC motors, servos, buttons, sensors,
 * I2C communication, OLED display, and WiFi functionality, allowing control and interaction
 * with the MatrixMiniR4 system.
 */
class MatrixMiniR4
{
public:
    MatrixMiniR4();
    bool begin();

    // Power
    MiniR4Power PWR; ///< Controller Power management

    // DC Motor
    MiniR4DC<1> M1; ///< Port M1 DC 5V Motor
    MiniR4DC<2> M2; ///< Port M2 DC 5V Motor
    MiniR4DC<3> M3; ///< Port M3 DC 5V Motor
    MiniR4DC<4> M4; ///< Port M4 DC 5V Motor
	
	// DC Drive Motor
	MiniR4DriveDC<1> DriveDC;	//Drive Two DC 5V Motor

    // Servo
    MiniR4RC<1> RC1; ///< Port RC1 RC 5V Servo
    MiniR4RC<2> RC2; ///< Port RC2 RC 5V Servo
    MiniR4RC<3> RC3; ///< Port RC3 RC 5V Servo
    MiniR4RC<4> RC4; ///< Port RC4 RC 5V Servo

    // Button
    MiniR4BTN<2> BTN_UP; ///< Up Button
    MiniR4BTN<1> BTN_DOWN; ///< Down Button

    // RGB LED (GPIO 7)
    MiniR4LED LED; ///< Built-in RGB LED

    // Motion
    MiniR4Motion Motion; ///< Built-in 6-axis motion sensor

    // Buzzer
    MiniR4BUZZER Buzzer;  ///< Built-in Buzzer

    // OLED
    Adafruit_SSD1306 OLED;  ///< Built-in OLED

    // WiFi
    CWifi WiFi; ///< WiFiS3 Object

    // Bluetooth
    // TODO: Add Bluetooth

    // I2C
    MiniR4I2C<-1, &Wire> I2C0; ///< Port A3
    MiniR4I2C<0, &Wire1> I2C1; ///< Port I2C1
    MiniR4I2C<1, &Wire1> I2C2; ///< Port I2C2
    MiniR4I2C<2, &Wire1> I2C3; ///< Port I2C3
    MiniR4I2C<3, &Wire1> I2C4; ///< Port I2C4

    // Digital I/O, PWM
    MiniR4PWM<3, 2>   D1; ///< Port D1 (p3, p2)(only LEFT support PWM)
    MiniR4PWM<5, 4>   D2; ///< Port D2 (p5, p4)(only LEFT support PWM)
    MiniR4PWM<12, 11> D3; ///< Port D3 (p12, p11)(only RIGHT support PWM)
    MiniR4PWM<13, 10> D4; ///< Port D4 (p13, p10)(only RIGHT support PWM)

    // Analog I/O, I2C0(Wire)
    MiniR4DAC<PIN_A1, PIN_A0>    A1; ///< Port A1(only RIGHT support DAC)
    MiniR4Analog<PIN_A3, PIN_A2> A2; ///< Port A2
    MiniR4Analog<PIN_A4, PIN_A5> A3; ///< Port A3

    // Uart
    UART Uart = UART(UART2_TX_PIN, UART2_RX_PIN); ///< UART Port, Or using Serial1

    // PS2
    PS2X PS2;  ///< MJ2 or PS2 (4p DAT CMD to D2, 2p CS CLK to D3)

    // VernierLib
    MiniR4VernierLib Vernier; ///< Vernier sensor support

    // Vision
    MiniR4SmartCamReader Vision;  ///< mVision (UART 9600)

private:
};

extern MatrixMiniR4 MiniR4;

#endif   // MATRIXMINIR4_H