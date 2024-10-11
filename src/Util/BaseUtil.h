/**
 * @file BaseUtil.h
 * @brief MiniR4 low level functions.
 * @author MATRIX Robotics
 */

#ifndef BASEUTIL_H
#define BASEUTIL_H

#include <stdint.h>

/**
 * @brief MiniR4 low level functions.
 */
class BaseUtil
{
public:
    template<typename T> static T map(T x, T in_min, T in_max, T out_min, T out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    template<typename T> static T clamp(T value, T min_value, T max_value)
    {
        if (value < min_value)
            return min_value;
        else if (value > max_value)
            return max_value;
        else
            return value;
    }

private:
};

#endif   // BASEUTIL_H
