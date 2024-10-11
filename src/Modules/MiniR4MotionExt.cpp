/**
 * @file MiniR4MotionExt.cpp
 * @brief Handling Matrix Motion Sensor functions.
 * @author MATRIX Robotics
 */

#include "MiniR4MotionExt.h"

bool MatrixMotion::begin()
{
    _pWire->begin();
    i2cMUXSelect();
    delay(50);
    if (i2cReadData(Device_ID) == 0x44) {
        i2cWriteData(Device_CONFIG, 0x02);   // reset
        delay(500);
        i2cWriteData(Device_CONFIG, 0x01);   // enable
        return true;
    } else {
        return false;
    }
}

int MatrixMotion::getRoll()
{
    i2cMUXSelect();
    int data = (int16_t)(i2cReadData(ROLL_H) << 8 | i2cReadData(ROLL_L));
    return data;
}

int MatrixMotion::getPitch()
{
    i2cMUXSelect();
    int data = (int16_t)(i2cReadData(PITCH_H) << 8 | i2cReadData(PITCH_L));
    return data;
}

int MatrixMotion::getYaw()
{
    i2cMUXSelect();
    int data = (int16_t)(i2cReadData(YAW_H) << 8 | i2cReadData(YAW_L));
    return data;
}

int MatrixMotion::getGyro(AxisType axis)
{
    i2cMUXSelect();
    int data = 0;
    switch (axis) {
    case x: data = (int16_t)(i2cReadData(GYRO_X_H) << 8 | i2cReadData(GYRO_X_L)); break;
    case y: data = (int16_t)(i2cReadData(GYRO_Y_H) << 8 | i2cReadData(GYRO_Y_L)); break;
    case z: data = (int16_t)(i2cReadData(GYRO_Z_H) << 8 | i2cReadData(GYRO_Z_L)); break;
    default: break;
    }
    return data;
}

int MatrixMotion::getAccel(AxisType axis)
{
    i2cMUXSelect();
    int data = 0;
    switch (axis) {
    case x: data = (int16_t)(i2cReadData(ACCEL_X_H) << 8 | i2cReadData(ACCEL_X_L)); break;
    case y: data = (int16_t)(i2cReadData(ACCEL_Y_H) << 8 | i2cReadData(ACCEL_Y_L)); break;
    case z: data = (int16_t)(i2cReadData(ACCEL_Z_H) << 8 | i2cReadData(ACCEL_Z_L)); break;
    default: break;
    }
    return data;
}

void MatrixMotion::i2cMUXSelect()
{
    if (_ch < 0) return;   // no MUX
    _pWire->beginTransmission(ADDR_PCA954X);
    _pWire->write((1 << _ch));
    _pWire->endTransmission(1);
    delayMicroseconds(300);
}

uint8_t MatrixMotion::i2cReadData(MotionRegType reg)
{
    _pWire->beginTransmission(MatrixMotion_ADDR);
    _pWire->write(reg);
    _pWire->endTransmission(1);

    delay(1);
    _pWire->requestFrom(MatrixMotion_ADDR, 1);
    delay(1);

    return _pWire->read();
}

void MatrixMotion::i2cWriteData(MotionRegType reg, uint8_t data)
{

    _pWire->beginTransmission(MatrixMotion_ADDR);

    _pWire->write(reg);
    _pWire->write(data);

    _pWire->endTransmission(1);
}
