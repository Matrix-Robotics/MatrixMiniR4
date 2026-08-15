/**
 * @file MiniR4_MJ5.h
 * @brief A library for interfacing with the MATRIX MJ5 Joystick via I2C.
 * 
 * @author MATRIX Robotics
 * @version 1.0
 * @date 2026
 * @license MIT License
 */
#ifndef _MiniR4_MJ5_H_
#define _MiniR4_MJ5_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#    define ADDR_PCA954X 0x70
#endif

// ─── MJ5 Device I2C Address ──────────────────────────────────────────────────
#define MJ5_I2C_ADDR    0x52

// ─── Register Map ────────────────────────────────────────────────────────────
#define MJ5_REG_STATUS  0x00  ///< Device status (Bit0: PS2 connected)
#define MJ5_REG_LX      0x01  ///< Left  stick X axis (0~255, 128=center)
#define MJ5_REG_LY      0x02  ///< Left  stick Y axis (0~255, 128=center)
#define MJ5_REG_RX      0x03  ///< Right stick X axis (0~255, 128=center)
#define MJ5_REG_RY      0x04  ///< Right stick Y axis (0~255, 128=center)
#define MJ5_REG_BTN_L   0x05  ///< Button state low  byte (bits 0~7)
#define MJ5_REG_BTN_H   0x06  ///< Button state high byte (bits 8~15)

// ─── Button Bitmask ──────────────────────────────────────────────────────────
#define PSB_SELECT      0x0001  ///< SELECT
#define PSB_L3          0x0002  ///< Left  stick push
#define PSB_R3          0x0004  ///< Right stick push
#define PSB_START       0x0008  ///< START
#define PSB_PAD_UP      0x0010  ///< D-pad Up
#define PSB_PAD_RIGHT   0x0020  ///< D-pad Right
#define PSB_PAD_DOWN    0x0040  ///< D-pad Down
#define PSB_PAD_LEFT    0x0080  ///< D-pad Left
#define PSB_L2          0x0100  ///< L2
#define PSB_R2          0x0200  ///< R2
#define PSB_L1          0x0400  ///< L1
#define PSB_R1          0x0800  ///< R1
#define PSB_TRIANGLE    0x1000  ///< △ (Green)
#define PSB_CIRCLE      0x2000  ///< ○ (Red)
#define PSB_CROSS       0x4000  ///< ✕ (Blue)
#define PSB_SQUARE      0x8000  ///< □ (Pink)

// ─── Axis center / deadzone default ──────────────────────────────────────────
#define MJ5_AXIS_CENTER      128
#define MJ5_DEADZONE_DEFAULT 8

/**
 * @class MatrixMJ5
 * @brief A class for MATRIX MJ5 joystick adapter.
 */
class MatrixMJ5 {
    private:
        uint8_t  _status;
        uint8_t  _lx, _ly;
        uint8_t  _rx, _ry;
        uint16_t _buttons;
        uint16_t _prevButtons;
        void i2cMUXSelect();
        static int8_t mapAxis(uint8_t raw, uint8_t deadzone);

    public:
        uint8_t  _ch = 0;    ///< I2C multiplexer channel
        TwoWire* _pWire;     ///< Pointer to Wire object

        /**
         * @brief Initialize the MJ5 Joystick
         */
        void begin();

        /**
         * @brief Burst-read all 7 registers (0x00~0x06).
         *        Call once per loop before reading any axis / button values.
         * @return true if MJ5 controller is connected, false otherwise.
         */
        bool update();

        /**
         * @brief Return true if the MJ5 controller is detected.
         */
        bool isConnected() const;

        /**
         * @brief Raw axis value (0~255, 127 = center).
         */
        uint8_t getLX() const;
        uint8_t getLY() const;
        uint8_t getRX() const;
        uint8_t getRY() const;

        /**
         * @brief Raw 16-bit button bitmask. Use PSB_* macros to test bits.
         */
        uint16_t getButtons() const;

        /**
         * @brief Return true while a button is held.
         * @param mask One or more PSB_* values OR-ed together.
         */
        bool Button(uint16_t mask) const;

};

#endif // _MiniR4_MJ5_H_
