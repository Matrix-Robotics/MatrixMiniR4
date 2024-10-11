/**
 * @file MiniR4Motion.h
 * @brief Handling Built-in IMU functions.
 * @author MATRIX Robotics
 */
#ifndef MINIR4MOTION_H
#define MINIR4MOTION_H

#include "MMLower.h"

/**
 * @brief Class for motion sensing using an IMU (Inertial Measurement Unit).
 *
 * Note: This Class is for Controller built-in IMU (PU6050) control by Lower MCU (STM32)
 * 
 * This class provides methods to retrieve gyro, accelerometer, and Euler angles 
 * data from the IMU. It also allows resetting the IMU values to zero.
 */
class MiniR4Motion
{
public:
    MiniR4Motion() {}

    enum class AxisType
    {
        X,
        Y,
        Z,
        Roll,
        Pitch,
        Yaw
    };

    /**
     * @brief Gets the gyro value for a specified axis.
     *
     * @param axis The axis for which to retrieve the gyro value (X, Y, Z).
     * @return The gyro value for the specified axis.
     */
    double getGyro(AxisType axis)
    {
        double x = 0, y = 0, z = 0;
        mmL.GetIMUGyro(x, y, z);

        if (axis == AxisType::X)
            return x;
        else if (axis == AxisType::Y)
            return y;
        else if (axis == AxisType::Z)
            return z;
        else
            return 0;
    }

    /**
     * @brief Gets the accelerometer value for a specified axis.
     *
     * @param axis The axis for which to retrieve the accelerometer value (X, Y, Z).
     * @return The accelerometer value for the specified axis.
     */
    double getAccel(AxisType axis)
    {
        double x = 0, y = 0, z = 0;
        mmL.GetIMUAcc(x, y, z);

        if (axis == AxisType::X)
            return x;
        else if (axis == AxisType::Y)
            return y;
        else if (axis == AxisType::Z)
            return z;
        else
            return 0;
    }

    /**
     * @brief Gets the Euler angle for a specified axis.
     *
     * @param axis The axis for which to retrieve the Euler angle (Roll, Pitch, Yaw).
     * @return The Euler angle for the specified axis.
     */
    int16_t getEuler(AxisType axis)
    {
        int16_t roll = 0, pitch = 0, yaw = 0;
        mmL.GetIMUEuler(roll, pitch, yaw);

        if (axis == AxisType::Roll)
            return roll;
        else if (axis == AxisType::Pitch)
            return pitch;
        else if (axis == AxisType::Yaw)
            return yaw;
        else
            return -999;
    }

    /**
     * @brief Resets the IMU values to zero.
     *
     * @return True if the reset operation was successful, false otherwise.
     */
    bool resetIMUValues(void) { return (mmL.SetIMUToZero() == MMLower::RESULT::OK); }

private:
};

#endif   // MINIR4MOTION_H
