/**
 * @file MiniR4Digital.h
 * @brief Handling MiniR4.Dn digital pin functions.
 * @author MATRIX Robotics
 */
#ifndef MiniR4Digital_H
#define MiniR4Digital_H

#include "Sensors/MiniR4HC04.h"
#include "Sensors/MiniR4_DHT11.h"
#include "Sensors/MiniR4_DS18B20.h"
#include "Sensors/MiniR4_Grove_US.h"
// #include "Sensors/MiniR4_Grove_TM1637.h"
#include <Arduino.h>

/**
 * @brief Class for handling digital input and output operations.
 *
 * This template class provides methods for reading and writing digital
 * signals to two specified pins, along with instantiating various sensor
 * classes that can be connected to these pins.
 *
 * @tparam PIN1 The first pin number.
 * @tparam PIN2 The second pin number.
 */
template<uint8_t PIN1, uint8_t PIN2> class MiniR4Digital
{
public:
    MiniR4Digital()
    {
        _pin1 = PIN1;
        _pin2 = PIN2;
    }

    /**
     * @brief Reads the state of the first pin.
     *
     * Configures the pin as an input and optionally enables the pull-up resistor.
     * 
     * @param pullup True to enable the internal pull-up resistor, false otherwise.
     * @return The digital state of the first pin (HIGH or LOW).
     */
    bool getL(bool pullup = false)
    {
        if (pullup) {
            pinMode(_pin1, INPUT_PULLUP);
        } else {
            pinMode(_pin1, INPUT);
        }
        return digitalRead(_pin1);
    }

    /**
     * @brief Reads the state of the second pin.
     *
     * Configures the pin as an input and optionally enables the pull-up resistor.
     * 
     * @param pullup True to enable the internal pull-up resistor, false otherwise.
     * @return The digital state of the second pin (HIGH or LOW).
     */
    bool getR(bool pullup = false)
    {
        if (pullup) {
            pinMode(_pin2, INPUT_PULLUP);
        } else {
            pinMode(_pin2, INPUT);
        }
        return digitalRead(_pin2);
    }

    /**
     * @brief Sets the level of the first pin.
     *
     * Configures the pin as an output and writes the specified level to it.
     * 
     * @param level The level to write to the pin (HIGH or LOW). Default is HIGH.
     */
    void setL(bool level = HIGH)
    {
        pinMode(_pin1, OUTPUT);
        digitalWrite(_pin1, level);
    }

    /**
     * @brief Sets the level of the second pin.
     *
     * Configures the pin as an output and writes the specified level to it.
     * 
     * @param level The level to write to the pin (HIGH or LOW). Default is HIGH.
     */
    void setR(bool level = HIGH)
    {
        pinMode(_pin2, OUTPUT);
        digitalWrite(_pin2, level);
    }

    /**
     * @brief Toggles the state of the first pin.
     *
     * Configures the pin as an output and toggles its current state.
     */
    void toggleL()
    {
        pinMode(_pin1, OUTPUT);
        digitalWrite(_pin1, !digitalRead(_pin1));
    }

    /**
     * @brief Toggles the state of the second pin.
     *
     * Configures the pin as an output and toggles its current state.
     */
    void toggleR()
    {
        pinMode(_pin2, OUTPUT);
        digitalWrite(_pin2, !digitalRead(_pin2));
    }

    MiniR4HC04<PIN1, PIN2> US; // MATRIX Ultrasonic sensor instance.
    MiniR4DHT11<PIN1, PIN2> DHT11; // Grove DHT11 sensor instance.
    MiniR4DS18B20<PIN1, PIN2> DS18B20; // Grove DS18B20 temperature sensor instance.
    MiniR4_Grove_US<PIN1, PIN2> GroveUS; // Grove ultrasonic sensor(ranger) instance.
    // MiniR4_Grove_TM1637<PIN1, PIN2> GroveTM1637; // Grove 7-segment display instance (TBD).
	
	MiniR4DHT11<PIN2, PIN1> MXDHT; // MATRIX DHT11 temperature and humidity sensor instance.
    MiniR4DS18B20<PIN2, PIN1> MXOnewireDT; // MATRIX DS18B20 temperature sensor instance.

private:
    uint8_t _pin1;
    uint8_t _pin2;
};

#endif   // MiniR4Digital_H
