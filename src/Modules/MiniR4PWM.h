#ifndef MiniR4PWM_H
#define MiniR4PWM_H

#include "MiniR4Digital.h"
#include <Arduino.h>

template<uint8_t PIN1, uint8_t PIN2> class MiniR4PWM : public MiniR4Digital<PIN1, PIN2>
{
public:
    MiniR4PWM()
    {
        _pin1 = PIN1;
        _pin2 = PIN2;
    }

    void setPWML(uint8_t level)
    {
        pinMode(_pin1, OUTPUT);
        analogWrite(_pin1, level);
    }

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
