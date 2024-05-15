#ifndef MiniR4DAC_H
#define MiniR4DAC_H

#include "MiniR4Analog.h"
#include <Arduino.h>

template<uint8_t PIN1, uint8_t PIN2> class MiniR4DAC : public MiniR4Analog<PIN1, PIN2>
{
public:
    MiniR4DAC()
    {
        _pin1 = PIN1;
        _pin2 = PIN2;
    }

    /* only arduinoR4 A0 support DAC  */
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
