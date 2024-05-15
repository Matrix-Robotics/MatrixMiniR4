#ifndef _MiniR4MotionExt_H_
#define _MiniR4MotionExt_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#    define ADDR_PCA954X 0x70
#endif

#define MatrixMotion_ADDR 0x23

typedef enum __AxisType
{
    x = 0,
    y,
    z
} AxisType;

class MatrixMotion
{
private:
    typedef enum __MotionRegType
    {
        Device_ID = 1,
        Device_CONFIG,
        ROLL_L,
        ROLL_H,
        PITCH_L,
        PITCH_H,
        YAW_L,
        YAW_H,
        GYRO_X_L,
        GYRO_X_H,
        GYRO_Y_L,
        GYRO_Y_H,
        GYRO_Z_L,
        GYRO_Z_H,
        ACCEL_X_L,
        ACCEL_X_H,
        ACCEL_Y_L,
        ACCEL_Y_H,
        ACCEL_Z_L,
        ACCEL_Z_H,
    } MotionRegType;

    uint8_t i2cReadData(MotionRegType reg);
    void    i2cMUXSelect();
    void    i2cWriteData(MotionRegType reg, uint8_t data);

public:
    int8_t   _ch = 0;
    TwoWire* _pWire;

    bool begin();

    int getRoll();
    int getPitch();
    int getYaw();

    int getGyro(AxisType axis);
    int getAccel(AxisType axis);
};

#endif
