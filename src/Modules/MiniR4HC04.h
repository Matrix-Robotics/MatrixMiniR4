#ifndef MINIR4HC04_H
#define MINIR4HC04_H

#include <Arduino.h>

template<uint8_t PIN1, uint8_t PIN2> class MiniR4HC04
{
public:
    MiniR4HC04()
    {
        _trigPin = PIN1;
        _echoPin = PIN2;
    }

    float getDistance(void)
    {
        pinMode(_trigPin, OUTPUT);
        pinMode(_echoPin, INPUT);
        digitalWrite(_trigPin, LOW);
        delayMicroseconds(2);

        digitalWrite(_trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(_trigPin, LOW);

        float duration = pulseIn(_echoPin, HIGH, 26500);   // timeout = 450cm

        if (duration == 0) {
            delayMicroseconds(100);
            // sensor timeout
            return -1;
        } else if (duration > 0 && duration < 294) {
            return 5.0;
        } else {
            return (0.017 * duration);
        }
    }


private:
    uint8_t _trigPin;
    uint8_t _echoPin;
};

#endif   // MINIR4HC04_H
