/**
 * DHT11.h DHT11.cpp
 * Header file for the DHT11 library, providing functionalities to interface with
 * the DHT11 temperature & humidity sensor.
 *
 * Author: Dhruba Saha
 * Version: 2.1.0
 * License: MIT
 * 241016 Anthony: 
 * Add float temp return.
 */
#ifndef MINIR4_DHT11_H
#define MINIR4_DHT11_H

#include <Arduino.h>

template <uint8_t PIN1, uint8_t PIN2> class MiniR4DHT11 {
public:
    MiniR4DHT11() {
        _pin = PIN2; //目前都在DI_R位置，未來可以改成LEFT跟MATRIX現行感應器一樣
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, HIGH);
    }

    static const int ERROR_CHECKSUM = 254;   // Error code for checksum mismatch.
    static const int ERROR_TIMEOUT = 253;    // Error code for timeout.
	// static const int ERROR_NOT_READY = 252;  // Error code for not ready (delay not passed).
    static const int TIMEOUT_DURATION = 1000; // Timeout duration in milliseconds.

    /**
     * Sets the delay between consecutive sensor readings.
     * Default is 250ms if not set.
     */
    void setDelay(unsigned long delay) {
        _delayMS = delay;
    }

    /**
     * Reads and returns the temperature from the DHT11 sensor.
     * 
     * @return Temperature in Celsius, or error codes if reading fails.
     */
    float readTemperature() {
        byte data[5];
        int error = readRawData(data);
        if (error != 0) {
            return error;
        }

        float t = (float)data[3] / 10;
        return data[2] + t;
    }

    /**
     * Reads and returns the humidity from the DHT11 sensor.
     * 
     * @return Humidity percentage, or error codes if reading fails.
     */
    int readHumidity() {
        byte data[5];
        int error = readRawData(data);
        if (error != 0) {
            return error;
        }

        return data[0];
    }

    /**
     * Reads temperature and humidity.
     * 
     * @param temperature Reference to store temperature.
     * @param humidity Reference to store humidity.
     * @return 0 on success, otherwise error code.
     */
    int readTemperatureHumidity(int &temperature, int &humidity) {
        byte data[5];
        int error = readRawData(data);
        if (error != 0) {
            return error;
        }
		
        humidity = data[0]; 
        float t = (float)data[3] / 10;
        temperature = data[2] + t;
        return 0;
    }

    /**
     * Converts error codes into human-readable strings.
     * 
     * @param errorCode Error code to convert.
     * @return String describing the error.
     */
    String getErrorString(int errorCode) {
        switch (errorCode) {
            case ERROR_TIMEOUT:
                return "Error 253: Timeout reading from DHT11.";
            case ERROR_CHECKSUM:
                return "Error 254: Checksum mismatch reading from DHT11.";
			// case ERROR_NOT_READY:
                // return "Error 252: Not ready to read from DHT11. Delay not passed.";
            default:
                return "Error: Unknown.";
        }
    }

private:
    int _pin;                    // Pin number for the DHT11 sensor.
    unsigned long _delayMS = 250; // Delay between readings.

    /**
     * Sends a start signal to the DHT11 sensor.
     */
    void startSignal() {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);
        delay(18);
        digitalWrite(_pin, HIGH);
        delayMicroseconds(40);
        pinMode(_pin, INPUT);
    }

    /**
     * Reads raw data from the DHT11 sensor.
     * 
     * @param data Array to store the raw data.
     * @return 0 on success, otherwise error code.
     */
    int readRawData(byte data[5]) {
        delay(_delayMS);
		// if (millis() - _lastReadTime < _delayMS) {
            // return ERROR_NOT_READY; // if not reach delayMS, bypass.
        // }
		
        startSignal();
        unsigned long timeout_start = millis();

        while (digitalRead(_pin) == HIGH) {
            if (millis() - timeout_start > TIMEOUT_DURATION) {
				return ERROR_TIMEOUT;
            }
        }

        delayMicroseconds(80);
        if (digitalRead(_pin) == HIGH) {
            delayMicroseconds(80);
            for (int i = 0; i < 5; i++) {
                data[i] = readByte();
            }

            if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
				// _lastReadTime = millis(); // update daley timer
				return 0; // Success
            } else {
                return ERROR_CHECKSUM;
            }
        }
        return ERROR_TIMEOUT;
    }

    /**
     * Reads a byte of data from the sensor.
     * 
     * @return The byte read from the sensor.
     */
    byte readByte() {
        byte value = 0;
        for (int i = 0; i < 8; i++) {
            while (digitalRead(_pin) == LOW);
            delayMicroseconds(30);
            if (digitalRead(_pin) == HIGH) {
                value |= (1 << (7 - i));
            }
            while (digitalRead(_pin) == HIGH);
        }
        return value;
    }
};

#endif // MINIR4_DHT11_H