#ifndef MINIR4BUZZER_H
#define MINIR4BUZZER_H

#include "MMLower.h"

class MiniR4BUZZER
{
public:
    MiniR4BUZZER() {}

    void begin(uint8_t pin) { _pin = pin; }

    void Tone(uint16_t frequency, uint32_t duration) { tone(_pin, frequency, duration); }
    void NoTone(void) { noTone(_pin); }

private:
    uint8_t _pin;
};

#endif   // MINIR4BUZZER_H
