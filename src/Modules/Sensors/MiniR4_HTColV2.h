/**
 * @file MiniR4HTColV2.h
 * @brief Handling HiTechnic NXT Color Sensor V2 functions for MiniR4.
 * @author https://github.com/a10036gt/Arduino-HTColV2, MATRIX Robotics
 */
#ifndef _MiniR4HTColV2_H_
#define _MiniR4HTColV2_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#define ADDR_PCA954X 0x70
#endif

#define HTCOLV2_ADDRESS 0x01

// Register definitions
#define HTCOLV2_DATA_REG    0x42

// Mode definitions
#define HTCOLV2_MODE_RGB    0x00
#define HTCOLV2_MODE_PASSIVE 0x01
#define HTCOLV2_MODE_RAW    0x03

/**
 * @brief Structure to hold HSV color values
 */
struct HSVColor {
    float hue; ///< Hue (0-360)
    float saturation; ///< Saturation (0-100)
    float value; ///< Value/Brightness (0-100)
};

/**
 * @brief Class for HiTechnic NXT Color Sensor V2 functions.
 */
class HTColV2
{
private:
    uint8_t _ver = 1;
    uint8_t _colorData[4]; ///< Buffer to store color data [Color Number, R, G, B]
    HSVColor _hsvData;     ///< HSV color values
    
    void i2cMUXSelect();
    void i2cWriteData(uint8_t data);
    uint32_t i2cReadData(int reg, int num);
    
    // Helper functions for RGB to HSV conversion
    float max3(float a, float b, float c);
    float min3(float a, float b, float c);
    void updateRGBData();
    void calculateHSV();

public:
    uint8_t _ch = 0;     ///< I2C multiplexer channel
    TwoWire *_pWire;     ///< Pointer to Wire object
    
    /**
     * @brief Initialize the color sensor
     */
    void begin();
    
    /**
     * @brief Set the sensor mode
     * @param mode Sensor mode (RGB=0, PASSIVE=1, RAW=3)
     */
    void setMode(uint8_t mode = HTCOLV2_MODE_RGB);
    
    /**
     * @brief Get the color number detected by the sensor
     * @return Color number (0-17, different colors have different numbers)
     */
    uint8_t getColorNumber();
    
    /**
     * @brief Get the red component value
     * @return Red value (0-255)
     */
    uint8_t getR();
    
    /**
     * @brief Get the green component value
     * @return Green value (0-255)
     */
    uint8_t getG();
    
    /**
     * @brief Get the blue component value
     * @return Blue value (0-255)
     */
    uint8_t getB();
    
    /**
     * @brief Get all RGB values at once
     * @param r Reference to store red value
     * @param g Reference to store green value  
     * @param b Reference to store blue value
     */
    void getRGB(uint8_t &r, uint8_t &g, uint8_t &b);
    
    /**
     * @brief Get the hue value (H in HSV)
     * @return Hue value (0-360 degrees)
     */
    float getH();
    
    /**
     * @brief Get the saturation value (S in HSV)
     * @return Saturation value (0-100%)
     */
    float getS();
    
    /**
     * @brief Get the brightness/value (V in HSV)
     * @return Brightness value (0-100%)
     */
    float getV();
    
    /**
     * @brief Get all HSV values at once
     * @param h Reference to store hue value
     * @param s Reference to store saturation value
     * @param v Reference to store brightness value
     */
    void getHSV(float &h, float &s, float &v);
	
};

#endif