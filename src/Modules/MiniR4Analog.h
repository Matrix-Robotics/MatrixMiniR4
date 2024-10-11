/**
 * @file MiniR4Analog.h
 * @brief Handling the MiniR4.An functions.
 * @author MATRIX Robotics
 */

#ifndef MiniR4Analog_H
#define MiniR4Analog_H

#include "MiniR4Digital.h"
#include <Arduino.h>

/**
 * @brief A class template for handling analog input operations on two specified pins.
 * 
 * This class inherits from the MiniR4Digital class and provides methods
 * to read analog values from two designated pins.
 *
 * @tparam PIN1 The first pin number for analog input.
 * @tparam PIN2 The second pin number for analog input.
 */
template<uint8_t PIN1, uint8_t PIN2> class MiniR4Analog : public MiniR4Digital<PIN1, PIN2>
{
public:
    MiniR4Analog()
    {
        _pin1 = PIN1;
        _pin2 = PIN2;
    }

    /**
     * @brief Reads the analog value from the first pin.
     * 
     * This method sets the first pin as an input and reads the analog
     * value from it.
     * 
     * @return The analog reading from pin1.
     */
    int getAIL(void)
    {
        pinMode(_pin1, INPUT);
        return analogRead(_pin1);
    }

    /**
     * @brief Reads the analog value from the second pin.
     * 
     * This method sets the second pin as an input and reads the analog
     * value from it.
     * 
     * @return The analog reading from pin2.
     */
    int getAIR(void)
    {
        pinMode(_pin2, INPUT);
        return analogRead(_pin2);
    }

private:
    uint8_t _pin1;
    uint8_t _pin2;
};

#endif   // MiniR4Analog_H
