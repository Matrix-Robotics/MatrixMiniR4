/**
 * @file MiniR4_Grove_US.h
 * @brief A library for interfacing with the Grove Ultrasonic Ranger sensor.
 * 
 * This library provides functions to measure distances using the Grove Ultrasonic Ranger sensor.
 * It supports measuring in centimeters, millimeters, and inches.
 * 
 * Add: Merge into MiniR4.Dn instance.
 * 
 * @author LG, FrankieChu
 * @version 1.0
 * @date January 17, 2013
 * @copyright MIT License
 */

#ifndef MINIR4_Grove_US_H
#define MINIR4_Grove_US_H

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <Arduino.h>

#ifdef ARDUINO_ARCH_STM32F4

static uint32_t MicrosDiff(uint32_t begin, uint32_t end) {
  return end - begin;
}

static uint32_t pulseIn(uint32_t pin, uint32_t state, uint32_t timeout = 1000000 L) {
  uint32_t begin = micros();

  // wait for any previous pulse to end
  while (digitalRead(pin))
    if (MicrosDiff(begin, micros()) >= timeout) {
      return 0;
    }

  // wait for the pulse to start
  while (!digitalRead(pin))
    if (MicrosDiff(begin, micros()) >= timeout) {
      return 0;
    }
  uint32_t pulseBegin = micros();

  // wait for the pulse to stop
  while (digitalRead(pin))
    if (MicrosDiff(begin, micros()) >= timeout) {
      return 0;
    }
  uint32_t pulseEnd = micros();

  return MicrosDiff(pulseBegin, pulseEnd);
}

#endif // ARDUINO_ARCH_STM32F4

/**
 * @class MiniR4_Grove_US
 * @brief A class for interfacing with Grove Ultrasonic Ranger.
 * 
 * This class allows measuring distances in centimeters, millimeters, and inches using 
 * the Grove Ultrasonic Ranger sensor. It works by sending ultrasonic pulses and measuring 
 * the time taken for the pulse to return.
 * 
 * @tparam PIN1 Unused pin (for future use).
 * @tparam PIN2 Pin number where the sensor is connected.
 */
template <uint8_t PIN1, uint8_t PIN2> class MiniR4_Grove_US {
  public: 
  
  MiniR4_Grove_US() {
    _pin = PIN2; //目前在DI R，未來可改成DI_L與MATRIX Sensor系統同步
  }

  /**
   * @brief Measures the distance in centimeters.
   *
   * @param timeout Timeout in microseconds (default: 1 second).
   * @return The measured distance in centimeters.
   */
  long MeasureInCentimeters(uint32_t timeout = 1000000L) {
    long RangeInCentimeters;
    RangeInCentimeters = duration(timeout) / 29 / 2;
    return RangeInCentimeters;
  }

  /**
   * @brief Measures the distance in millimeters.
   *
   * @param timeout Timeout in microseconds (default: 1 second).
   * @return The measured distance in millimeters.
   */
  long MeasureInMillimeters(uint32_t timeout = 1000000L) {
    long RangeInMillimeters;
    RangeInMillimeters = duration(timeout) * (10 / 2) / 29;
    return RangeInMillimeters;
  }

  /**
   * @brief Measures the distance in inches.
   *
   * @param timeout Timeout in microseconds (default: 1 second).
   * @return The measured distance in inches.
   */
  long MeasureInInches(uint32_t timeout = 1000000L) {
    long RangeInInches;
    RangeInInches = duration(timeout) / 74 / 2;
    return RangeInInches;
  }

  private: 
  int _pin; //pin number of Arduino that is connected with SIG pin of Ultrasonic Ranger.
  long duration(uint32_t timeout = 1000000L) {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(_pin, HIGH);
    delayMicroseconds(5);
    digitalWrite(_pin, LOW);
    pinMode(_pin, INPUT);
    long duration;
    duration = pulseIn(_pin, HIGH, timeout);
    return duration;
  }
};

#endif // MINIR4_Grove_US_H