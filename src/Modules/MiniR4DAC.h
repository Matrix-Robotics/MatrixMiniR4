/**
 * @file MiniR4DAC.h
 * @brief Handling MiniR4.An DAC functions.
 * @author MATRIX Robotics
 */

#ifndef MiniR4DAC_H
#define MiniR4DAC_H

#include "MiniR4Analog.h"
#include <Arduino.h>

/**
 * @brief Class for Digital-to-Analog Converter (DAC) functionality.
 *
 * This template class provides methods to set a DAC level using specified pins.
 *
 * @tparam PIN1 The first pin used for DAC output.
 * @tparam PIN2 The second pin (not used for DAC in this implementation).
 */
template<uint8_t PIN1, uint8_t PIN2> class MiniR4DAC : public MiniR4Analog<PIN1, PIN2>
{
public:
    MiniR4DAC()
    {
        _pin1 = PIN1;
        _pin2 = PIN2;
    }

    /**
     * @brief Sets the Digital-to-Analog Converter (DAC) level.
     *
     * This function sets the DAC level using the specified pin.
     * Note: Only Arduino R4 A0 supports DAC.
     *
     * @param level The level to set for the DAC (0-255).
     */
    void setDACR(int level)
    {
        pinMode(_pin1, OUTPUT);
        analogWrite(_pin1, level);
    }

private:
    uint8_t _pin1;
    uint8_t _pin2;
};

#endif   // MiniR4DAC_H
