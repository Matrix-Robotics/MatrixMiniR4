#ifndef MiniR4Analog_H
#define MiniR4Analog_H

#include "MiniR4Digital.h"
#include <Arduino.h>

template<uint8_t PIN1, uint8_t PIN2> class MiniR4Analog : public MiniR4Digital<PIN1, PIN2>
{
public:
    MiniR4Analog()
    {
        _pin1 = PIN1;
        _pin2 = PIN2;
    }

    int getAIL(void)
    {
        pinMode(_pin1, INPUT);
        return analogRead(_pin1);
    }

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
