/**
 * @file MiniR4ColorSensorExt.h
 * @brief Matric Color Sensor functions.
 * @author MATRIX Robotics
 */

#ifndef _MiniR4ColorSensorExt_H_
#define _MiniR4ColorSensorExt_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#    define ADDR_PCA954X 0x70
#endif

#define MatrixColor_ADDR 0x22

typedef enum __ColorType
{
    R = 4,
    G,
    B,
    C,
    M,
    Y,
    K
} ColorType;

/**
 * @brief Class for interacting with a color sensor.
 * 
 * This class provides methods to initialize the sensor, configure settings,
 * and retrieve color values from the sensor.
 */
class MatrixColor
{
private:
    typedef enum __ColorRegType
    {
        Device_ID = 1,
        Device_CONFIG,
        Device_LIGHT,
        Device_RED,
        Device_GREEN,
        Device_BLUE,
        Device_CYAN,
        Device_MAGENTA,
        Device_YELLOW,
        Device_BLACK,
        Device_GRAY,
        Device_NUM_COLOR
    } ColorRegType;

    uint8_t setting = 0x0F;
    uint8_t i2cReadData(ColorRegType reg);
    void    i2cMUXSelect();
    void    i2cWriteData(ColorRegType reg, uint8_t data);

public:
    uint8_t  _ch = 0;
    TwoWire* _pWire;

    /**
     * @brief Initializes the color sensor.
     *
     * @return true if initialization was successful, false otherwise.
     */
    bool    begin();

    /**
     * @brief Sets the gamma correction state for the sensor.
     *
     * @param state true to enable gamma correction, false to disable it.
     */
    void    setGamma(bool state);

    /**
     * @brief Sets the light state and mode for the sensor.
     *
     * @param state true to turn on the light, false to turn it off.
     * @param mode The mode of the light (e.g., PWM).
     * @param pwm The PWM value to set the light brightness.
     */
    void    setLight(bool state, bool mode, uint8_t pwm);

    /**
     * @brief Gets the value of a specific color from the sensor.
     *
     * @param color The color type to retrieve.
     * @return The value of the specified color.
     */
    uint8_t getColor(ColorType color);

    /**
     * @brief Gets the grayscale value from the sensor.
     *
     * @return The grayscale value.
     */
    uint8_t getGrayscale();

    /**
    * @brief Gets the number of colors supported by the sensor.
    * Number of color as follow as below:
    * 0:Black, 1:White, 2:Cyan, 3:Ocean, 4:Blue, 5:Violet, 6:Magenta
    * 7:Raspberry, 8:Red, 9:Orange, 10:Yellow, 11:Spring, 12:Green, 13:Turquoise
    * @return The number of color values.
    */
    uint8_t getColorNumber();
};

#endif
