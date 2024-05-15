#ifndef MINIR4MOTION_H
#define MINIR4MOTION_H

#include "MMLower.h"

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

    bool resetIMUValues(void) { return (mmL.SetIMUToZero() == MMLower::RESULT::OK); }

private:
};

#endif   // MINIR4MOTION_H
