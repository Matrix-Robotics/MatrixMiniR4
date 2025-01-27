/**
 * @file MiniR4LaserSensorExt.h
 * @brief Handling MiniR4.I2Cn Matrix Laser Sensor functions.
 * @author MATRIX Robotics
 */
#ifndef _MiniR4LaserSensorExt_H_
#define _MiniR4LaserSensorExt_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#    define ADDR_PCA954X 0x70
#endif

#define MatrixLaser_ADDR 0x26

/**
 * @brief Class for interfacing with a laser distance sensor.
 *
 * This class provides methods to read distance measurements from a laser sensor
 * over I2C.
 */
class MatrixLaser
{
private:
    typedef enum __LaserRegType
    {
        Device_ID = 1,
        Device_CONFIG,
        Distance_H,
        Distance_L
    } LaserRegType;

    uint8_t i2cReadData(LaserRegType reg);
    void    i2cMUXSelect();
    void    i2cWriteData(LaserRegType reg, uint8_t data);

public:
    uint8_t  _ch = 0;
    TwoWire* _pWire;

    /**
     * @brief Initializes the laser sensor.
     *
     * @return True if initialization is successful, false otherwise.
     */
    bool     begin();

    /**
     * @brief Gets the distance measured by the laser sensor.
     *
     * @return The measured distance in millimeters, if value invaild will return 8191.
     */
    uint16_t getDistance();
};

#endif
