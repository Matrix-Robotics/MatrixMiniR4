/**
 * @file MiniR4_MXColorV3.h
 * @brief A library for interfacing with the MATRIX Color Seneor V3 sensor (TCS34725) via I2C.
 * 
 * This class allows reading color data from the MATRIX Color Seneor V3.
 * 
 * Add: Merge into MiniR4.I2C instance.
 * 
 * @author MATRIX Robotics
 * @version 1.0
 * @date 2024
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
		 * @brief Getting Red channel value from MATRIX Color V3 sensor.
		 *
		 * @return Red channel value.
		 */
		uint16_t getR();
		
		/**
		 * @brief Getting Green channel value from MATRIX Color V3 sensor.
		 *
		 * @return Green channel value.
		 */
		uint16_t getG();
		
		/**
		 * @brief Getting Bule channel value from MATRIX Color V3 sensor.
		 *
		 * @return Blue channel value.
		 */
		uint16_t getB();
		
		/**
		 * @brief Getting Clear channel value from MATRIX Color V3 sensor.
		 *
		 * @return Clear channel value.
		 */
		uint16_t getC();
		
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