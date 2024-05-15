/**
 * @file MatrixMiniR4.h
 * @author Zack Huang (zackhuang0513@gmail.com)
 * @brief Matrix Mini Arduino Library
 * @version 1.0.0
 * @date 2024-02-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MATRIXMINIR4_H
#define MATRIXMINIR4_H

#include "Modules/MiniR4Analog.h"
#include "Modules/MiniR4BTN.h"
#include "Modules/MiniR4Buzzer.h"
#include "Modules/MiniR4DAC.h"
#include "Modules/MiniR4DC.h"
#include "Modules/MiniR4ENC.h"
#include "Modules/MiniR4I2C.h"
#include "Modules/MiniR4LED.h"
#include "Modules/MiniR4Motion.h"
#include "Modules/MiniR4OLED.h"
#include "Modules/MiniR4PS2X_lib.h"
#include "Modules/MiniR4PWM.h"
#include "Modules/MiniR4Power.h"
#include "Modules/MiniR4RC.h"
#include "Modules/MiniR4SmartCamReader.h"
#include "Modules/MiniR4VernierLib.h"

#include <Arduino.h>
#include <WiFiS3.h>
#include <Wire.h>

#define MATRIXMINIR4_OLED_ADDRESS 0x3D

class MatrixMiniR4
{
public:
    MatrixMiniR4();
    bool begin();

    // Power
    MiniR4Power PWR;

    // DC Motor
    MiniR4DC<1> M1;
    MiniR4DC<2> M2;
    MiniR4DC<3> M3;
    MiniR4DC<4> M4;

    // Encoder
    MiniR4ENC<1> ENC1;
    MiniR4ENC<2> ENC2;
    MiniR4ENC<3> ENC3;
    MiniR4ENC<4> ENC4;

    // Servo
    MiniR4RC<1> RC1;
    MiniR4RC<2> RC2;
    MiniR4RC<3> RC3;
    MiniR4RC<4> RC4;

    // Button
    MiniR4BTN<1> BTN_UP;
    MiniR4BTN<2> BTN_DOWN;

    // RGB LED (GPIO 7)
    MiniR4LED LED;

    // Motion
    MiniR4Motion Motion;

    // Buzzer
    MiniR4BUZZER Buzzer;

    // OLED
    Adafruit_SSD1306 OLED;

    // WiFi
    CWifi WiFi;

    // Bluetooth
    // TODO: Add Bluetooth

    // I2C
    MiniR4I2C<-1, &Wire> I2C0;   // Wire0
    MiniR4I2C<0, &Wire1> I2C1;   // I2C Mux channel 0
    MiniR4I2C<1, &Wire1> I2C2;   // I2C Mux channel 1
    MiniR4I2C<2, &Wire1> I2C3;   // I2C Mux channel 2
    MiniR4I2C<3, &Wire1> I2C4;   // I2C Mux channel 3

    // Digital I/O, PWM
    MiniR4PWM<3, 2>   D1;   // only arduinoR4 D3 support PWM
    MiniR4PWM<5, 4>   D2;   // only arduinoR4 D5 support PWM
    MiniR4PWM<12, 11> D3;   // only arduinoR4 D11 support PWM
    MiniR4PWM<13, 10> D4;   // only arduinoR4 D10 support PWM

    // Analog I/O, I2C0(Wire)
    MiniR4DAC<PIN_A1, PIN_A0>    A1;   // only arduinoR4 A0 support DAC
    MiniR4Analog<PIN_A3, PIN_A2> A2;
    MiniR4Analog<PIN_A4, PIN_A5> A3;

    // Uart
    UART Uart = UART(UART2_TX_PIN, UART2_RX_PIN);

    // PS2
    PS2X PS2;

    // VernierLib
    MiniR4VernierLib Vernier;

    // Vision
    MiniR4SmartCamReader Vision;

private:
};

extern MatrixMiniR4 MiniR4;

#endif   // MATRIXMINIR4_H