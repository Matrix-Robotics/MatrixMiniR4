/**
 * @file MiniR4Buzzer.h
 * @brief Handling the MiniR4.Buzzer functions.
 * @author MATRIX Robotics
 */
#ifndef MINIR4BUZZER_H
#define MINIR4BUZZER_H

#include "MMLower.h"

/**
 * @brief A class for controlling a buzzer.
 * 
 * This class provides methods to generate tones and stop tones
 * using a specified pin on the Arduino.
 */
class MiniR4BUZZER
{
public:
    MiniR4BUZZER() {}

    /**
     * @brief Initializes the buzzer by setting the pin number. (Built Buzzer at Uno Pin 6)
     * 
     * @param pin The pin number connected to the buzzer.
     */
    void begin(uint8_t pin) { _pin = pin; }

    /**
     * @brief Generates a tone at the specified frequency for a given duration.
     * 
     * @param frequency The frequency of the tone in hertz.
     * @param duration The duration of the tone in milliseconds.
     */
    void Tone(uint16_t frequency, uint32_t duration) { tone(_pin, frequency, duration); }
    
    /**
     * @brief Stops the tone on the buzzer.
     */
    void NoTone(void) { noTone(_pin); }

private:
    uint8_t _pin;
};

#endif   // MINIR4BUZZER_H
