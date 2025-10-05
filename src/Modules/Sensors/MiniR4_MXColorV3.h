/**
 * @file MiniR4_MXColorV3.h
 * @brief A library for interfacing with the MATRIX Color Seneor V3 sensor (TCS34725) via I2C.
 * 
 * This class allows reading color data from the MATRIX Color Seneor V3.
 * 
 * Add: Merge into MiniR4.I2C instance.
 * 
 * @author MATRIX Robotics
 * @version 1.1
 * @date 2025
 * @license MIT License
 */
#ifndef _MiniR4_MXColorV3_H_
#define _MiniR4_MXColorV3_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#    define ADDR_PCA954X 0x70
#endif

// TCS34725 I2C Addr
#define TCS34725_ADDRESS          0x29
#define TCS34725_COMMAND_BIT      0x80

// TCS34725 Regs Addrs
#define TCS34725_ENABLE           0x00
#define TCS34725_ATIME            0x01
#define TCS34725_CONTROL          0x0F
#define TCS34725_ID               0x12
#define TCS34725_STATUS           0x13
#define TCS34725_CDATAL           0x14
#define TCS34725_RDATAL           0x16
#define TCS34725_GDATAL           0x18
#define TCS34725_BDATAL           0x1A

// Enable AEN PON
#define TCS34725_ENABLE_AEN       0x02
#define TCS34725_ENABLE_PON       0x01

// GAN 1x
#define TCS34725_GAIN_1X          0x00

// INTEGRATIONTIME 24ms
#define TCS34725_INTEGRATIONTIME_24MS  0xF6
#define TCS34725_INTEGRATIONTIME_50MS  0xEB

// Color ID
#define COLOR_BLACK      0
#define COLOR_VIOLET     1
#define COLOR_BLUE       3
#define COLOR_LIGHTBLUE  4
#define COLOR_GREEN      6
#define COLOR_YELLOW     7
#define COLOR_RED        9
#define COLOR_WHITE      10
#define COLOR_NONE       -1

/**
 * @class MatrixColorV3
 * @brief A class for MATRIX Color Seneor V3 sensor.
 *
 * This class allows reading distance data from the MATRIX Color Seneor V3 sensor.
 */
class MatrixColorV3{
	private:
		void i2cMUXSelect();
		void writeRegister(uint8_t reg, uint8_t value);
		uint16_t readRegister16(uint8_t reg);
		
		float _wb_R = 1.1;
		float _wb_G = 1.0;
		float _wb_B = 1.1;
		
		// HSV 快取
		float _cachedH;
		float _cachedS;
		float _cachedV;
		
		// 內部函數：計算 HSV
		void updateHSV();
	
	public:
		uint8_t _ch = 0;
		TwoWire* _pWire;
		
		/**
		 * @brief Initializes the MATRIX Color V3 sensor.
		 *
		 * @return true if initialization is successful, false otherwise.
		 */
		bool begin();
		
		/**
		 * @brief Setting White Balance the MATRIX Color V3 sensor.
		 * @param white_r Raw value of RED channel of white brick or object.
		 * @param white_g Raw value of GREEN channel of white brick or object.
		 * @param white_b Raw value of BULE channel of white brick or object.
		 */
		void setWhiteBalance(uint16_t white_r, uint16_t white_g, uint16_t white_b);
		
		/**
		 * @brief Getting Red channel value from MATRIX Color V3 sensor (normalized 0-255).
		 *
		 * @return Red channel value (0-255).
		 */
		int16_t getR();
		
		/**
		 * @brief Getting Green channel value from MATRIX Color V3 sensor (normalized 0-255).
		 *
		 * @return Green channel value (0-255).
		 */
		int16_t getG();
		
		/**
		 * @brief Getting Blue channel value from MATRIX Color V3 sensor (normalized 0-255).
		 *
		 * @return Blue channel value (0-255).
		 */
		int16_t getB();
		
		/**
		 * @brief Getting Clear channel value from MATRIX Color V3 sensor.
		 *
		 * @return Clear channel value (0-65535).
		 */
		int16_t getC();
		
		/**
		 * @brief Getting Hue value (色相) from MATRIX Color V3 sensor.
		 *
		 * @return Hue value (0-360 degrees). Returns -1 if color is undefined (grayscale).
		 */
		float getH();
		
		/**
		 * @brief Getting Saturation value (飽和度) from MATRIX Color V3 sensor.
		 *
		 * @return Saturation percentage (0-100%).
		 */
		float getS();
		
		/**
		 * @brief Getting Value/Brightness (明度) from MATRIX Color V3 sensor.
		 *
		 * @return Value percentage (0-100%).
		 */
		float getV();
		
		/**
		 * @brief Get Color ID for easy color recognition.
		 *
		 * @return Color ID:
		 *   -1 = No color (too dark or sensor error),
		 *    0 = Black,
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
		 * @brief Getting raw 16-bit Red channel value.
		 *
		 * @return Raw red channel value (0-65535).
		 */
		uint16_t getRaw_R();
		
		/**
		 * @brief Getting raw 16-bit Green channel value.
		 *
		 * @return Raw green channel value (0-65535).
		 */
		uint16_t getRaw_G();
		
		/**
		 * @brief Getting raw 16-bit Blue channel value.
		 *
		 * @return Raw blue channel value (0-65535).
		 */
		uint16_t getRaw_B();
		
		/**
		 * @brief Read and normalize RGB values.
		 * 
		 * More efficient than calling getR(), getG(), getB() separately.
		 * Reads raw sensor data and normalizes RGB based on Clear channel.
		 * 
		 * @param r Normalized Red (0-255)
		 * @param g Normalized Green (0-255)
		 * @param b Normalized Blue (0-255)
		 * @param c Clear channel (0-65535)
		 * 
		 * @note Returns RGB=0 if light is too dark (C<40)
		 */
		void readNormalizedRGB(int16_t &r, int16_t &g, int16_t &b, int16_t &c);
		
		/**
		 * @brief A method to calculate the Color Temperature.
		 *
		 * @return Color Temperature (K).
		 */
		float calcColorTemp(uint16_t r, uint16_t g, uint16_t b);
		
		/**
		 * @brief A method to calculate the Lux.
		 *
		 * @return Illuminance.
		 */
		float calcLux(uint16_t r, uint16_t g, uint16_t b);
		
		/**
		 * @brief A method to convert RGB to HSV.
		 *
		 * @return HSV value.
		 */
		void RGB2HSV(uint16_t r, uint16_t g, uint16_t b, float &h, float &s, float &v);
		
};

#endif