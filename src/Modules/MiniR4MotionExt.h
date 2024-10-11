/**
 * @file MiniR4MotionExt.h
 * @brief Handling Matrix Motion Sensor functions.
 * @author MATRIX Robotics
 */
#ifndef _MiniR4MotionExt_H_
#define _MiniR4MotionExt_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#define ADDR_PCA954X 0x70
#endif

#define MatrixMotion_ADDR 0x23

typedef enum __AxisType
{
    x = 0,
    y,
    z
} AxisType;

/**
 * @brief Class for motion sensing using a Matrix Motion sensor.
 *
 * Note: This Class is for Matrix Motion Sensor (LSM6DS3)
 *
 * This class provides methods to retrieve roll, pitch, yaw, gyro, and accelerometer
 * data from the motion sensor over I2C.
 */
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
    void i2cMUXSelect();
    void i2cWriteData(MotionRegType reg, uint8_t data);

public:
    int8_t _ch = 0;
    TwoWire *_pWire;

    /**
     * @brief Initializes the MatrixMotion sensor.
     *
     * @return True if the initialization was successful, false otherwise.
     */
    bool begin();

    /**
     * @brief Gets the roll angle from the motion sensor.
     *
     * @return The roll angle in degrees.
     */
    int getRoll();

    /**
     * @brief Gets the pitch angle from the motion sensor.
     *
     * @return The pitch angle in degrees.
     */
    int getPitch();

    /**
     * @brief Gets the yaw angle from the motion sensor.
     *
     * @return The yaw angle in degrees.
     */
    int getYaw();

    /**
     * @brief Gets the gyro value for a specified axis.
     *
     * @param axis The axis for which to retrieve the gyro value (x, y, z).
     * @return The gyro value for the specified axis.
     */
    int getGyro(AxisType axis);

    /**
     * @brief Gets the accelerometer value for a specified axis.
     *
     * @param axis The axis for which to retrieve the accelerometer value (x, y, z).
     * @return The accelerometer value for the specified axis.
     */
    int getAccel(AxisType axis);
};

#endif
