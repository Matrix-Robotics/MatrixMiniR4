/**
 * @file MiniR4_MJ5.cpp
 * @brief A library for interfacing with the MATRIX MJ5 Joystick via I2C.
 *
 * Register map (burst read 0x00~0x06, 7 bytes):
 *   0x00  Status   Bit0: PS2 connected
 *   0x01  LX       Left  stick X (0~255, 127=center)
 *   0x02  LY       Left  stick Y (0~255, 127=center)
 *   0x03  RX       Right stick X (0~255, 127=center)
 *   0x04  RY       Right stick Y (0~255, 127=center)
 *   0x05  BTN_L    Button low  byte (SELECT~PAD_LEFT)
 *   0x06  BTN_H    Button high byte (L2~SQUARE)
 *
 * @author MATRIX Robotics
 * @version 1.0
 * @date 2026
 * @license MIT License
 */
#include "MiniR4_MJ5.h"

void MatrixMJ5::begin() {
    _pWire->begin();
    _status      = 0;
    _lx          = MJ5_AXIS_CENTER;
    _ly          = MJ5_AXIS_CENTER;
    _rx          = MJ5_AXIS_CENTER;
    _ry          = MJ5_AXIS_CENTER;
    _buttons     = 0;
    _prevButtons = 0;
}

bool MatrixMJ5::update() {
    i2cMUXSelect();

    _pWire->beginTransmission(MJ5_I2C_ADDR);
    _pWire->write(MJ5_REG_STATUS);
    if (_pWire->endTransmission(false) != 0) {
        _status = 0;
        return false;
    }

    _pWire->requestFrom(MJ5_I2C_ADDR, (uint8_t)7);

    uint8_t buf[7];
    for (uint8_t i = 0; i < 7; i++) {
        if (!_pWire->available()) { _status = 0; return false; }
        buf[i] = _pWire->read();
    }

    _status      = buf[0];
    _prevButtons = _buttons;

    if (_status & 0x01) {
        _lx      = buf[1];
        _ly      = buf[2];
        _rx      = buf[3];
        _ry      = buf[4];
        _buttons = ((uint16_t)buf[6] << 8) | buf[5];
    } else {
        _lx      = MJ5_AXIS_CENTER;
        _ly      = MJ5_AXIS_CENTER;
        _rx      = MJ5_AXIS_CENTER;
        _ry      = MJ5_AXIS_CENTER;
        _buttons = 0;
    }

    return (_status & 0x01);
}

bool MatrixMJ5::isConnected() const {
    return (_status & 0x01) != 0;
}

uint8_t MatrixMJ5::getLX() const { return _lx; }
uint8_t MatrixMJ5::getLY() const { return _ly; }
uint8_t MatrixMJ5::getRX() const { return _rx; }
uint8_t MatrixMJ5::getRY() const { return _ry; }

uint16_t MatrixMJ5::getButtons() const { return _buttons; }

bool MatrixMJ5::Button(uint16_t mask) const {
    return (_buttons & mask) != 0;
}

void MatrixMJ5::i2cMUXSelect() {
    if (_ch < 0) return;
    _pWire->beginTransmission(ADDR_PCA954X);
    _pWire->write((1 << _ch));
    _pWire->endTransmission(1);
    delayMicroseconds(100);
}
