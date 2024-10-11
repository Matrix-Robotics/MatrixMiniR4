/**
 * @file MiniR4PWM.h
 * @brief Handling MiniR4.Dn PWM functions.
 * @author MATRIX Robotics
 */
#ifndef MiniR4PWM_H
#define MiniR4PWM_H

#include "MiniR4Digital.h"
#include <Arduino.h>

/**
 * @brief A class to handle PWM (Pulse Width Modulation) signals on two specified pins.
 *
 * This class inherits from MiniR4Digital and provides methods to set the PWM
 * levels for two separate pins.
 *
 * @tparam PIN1 The first PWM pin.
 * @tparam PIN2 The second PWM pin.
 */
template<uint8_t PIN1, uint8_t PIN2> class MiniR4PWM : public MiniR4Digital<PIN1, PIN2>
{
public:
    MiniR4PWM()
    {
        _pin1 = PIN1;
        _pin2 = PIN2;
    }

    /**
     * @brief Sets the PWM level for the first pin.
     * 
     * Note: Only D1 (p3) and D2 (p5) LEFT Pin Support PWM.
     *
     * @param level The PWM level to set (0-255).
     */
    void setPWML(uint8_t level)
    {
        pinMode(_pin1, OUTPUT);
        analogWrite(_pin1, level);
    }

    /**
     * @brief Sets the PWM level for the second pin.
     * 
     * Note: Only D3 (p11) and D4 (p10) RIGHT Pin Support PWM.
     *
     * @param level The PWM level to set (0-255).
     */
    void setPWMR(uint8_t level)
    {
        pinMode(_pin2, OUTPUT);
        analogWrite(_pin2, level);
    }

private:
    uint8_t _pin1;
    uint8_t _pin2;
};

#endif   // MiniR4PWM_H