#ifndef MiniR4Digital_H
#define MiniR4Digital_H

#include "MiniR4HC04.h"
#include <Arduino.h>

template<uint8_t PIN1, uint8_t PIN2> class MiniR4Digital
{
public:
    MiniR4Digital()
    {
        _pin1 = PIN1;
        _pin2 = PIN2;
    }

    bool getL(bool pullup = false)
    {
        if (pullup) {
            pinMode(_pin1, INPUT_PULLUP);
        } else {
            pinMode(_pin1, INPUT);
        }
        return digitalRead(_pin1);
    }

    bool getR(bool pullup = false)
    {
        if (pullup) {
            pinMode(_pin2, INPUT_PULLUP);
        } else {
            pinMode(_pin2, INPUT);
        }
        return digitalRead(_pin2);
    }

    void setL(bool level = HIGH)
    {
        pinMode(_pin1, OUTPUT);
        digitalWrite(_pin1, level);
    }

    void setR(bool level = HIGH)
    {
        pinMode(_pin2, OUTPUT);
        digitalWrite(_pin2, level);
    }

    void toggleL()
    {
        pinMode(_pin1, OUTPUT);
        digitalWrite(_pin1, !digitalRead(_pin1));
    }

    void toggleR()
    {
        pinMode(_pin2, OUTPUT);
        digitalWrite(_pin2, !digitalRead(_pin2));
    }

    MiniR4HC04<PIN1, PIN2> US;

private:
    uint8_t _pin1;
    uint8_t _pin2;
};

#endif   // MiniR4Digital_H
