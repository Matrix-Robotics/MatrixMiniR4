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

// Color ID definitions
#define COLOR_BLACK      0
#define COLOR_VIOLET     1
#define COLOR_BLUE       3
#define COLOR_LIGHTBLUE  4
#define COLOR_GREEN      6
#define COLOR_YELLOW     7
#define COLOR_RED        9
#define COLOR_WHITE      10
#define COLOR_NONE       -1

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
    
    // HSV cache
    float _cachedH;
    float _cachedS;
    float _cachedV;
    
    // Internal function: calculate HSV
    void updateHSV();

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
    * @brief (Deprecated) Gets the number of colors supported by the sensor.
    * Number of color as follow as below:
    * 0:Black, 1:White, 2:Cyan, 3:Ocean, 4:Blue, 5:Violet, 6:Magenta
    * 7:Raspberry, 8:Red, 9:Orange, 10:Yellow, 11:Spring, 12:Green, 13:Turquoise
    * @return The number of color values.
    */
    uint8_t getColorNumber_Deprecated();
    
    /**
     * @brief Getting Hue value (色相) from color sensor.
     *
     * @return Hue value (0-360 degrees).
     */
    float getH();
    
    /**
     * @brief Getting Saturation value (飽和度) from color sensor.
     *
     * @return Saturation percentage (0-100%).
     */
    float getS();
    
    /**
     * @brief Getting Value/Brightness (明度) from color sensor.
     *
     * @return Value percentage (0-100%).
     */
    float getV();
    
    /**
     * @brief Get Color ID for easy color recognition.
     *
     * @return Color ID:
     *   -1 = No color (too dark or sensor error),
     *    0 = Black (Not easy to detect),
     *    1 = Violet,
     *    3 = Blue,
     *    4 = Light Blue (Cyan),
     *    6 = Green,
     *    7 = Yellow,
     *    9 = Red,
     *   10 = White,
     */
    int8_t getColorID();
    
    /**
     * @brief A method to convert RGB to HSV.
     *
     * @param r Red value (0-255)
     * @param g Green value (0-255)
     * @param b Blue value (0-255)
     * @param h Hue output (0-360 degrees)
     * @param s Saturation output (0-100%)
     * @param v Value output (0-100%)
     */
    void RGB2HSV(uint8_t r, uint8_t g, uint8_t b, float &h, float &s, float &v);
};

#endif