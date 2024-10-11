/**
 * @file MiniR4_GroveI2C_BME280.h
 * @brief A library for interfacing with the BME280 sensor via I2C.
 * 
 * This library provides functions to read temperature, pressure, and humidity data from the BME280 sensor.
 * It also includes calibration data handling and altitude calculation based on pressure.
 * 
 * Add: Merge into MiniR4.I2C instance.
 * 
 * @author Lambor
 * @version 1.0
 * @date 2016
 * @copyright MIT License
 */
#ifndef _MiniR4_GroveI2C_BME280_H_
#define _MiniR4_GroveI2C_BME280_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#    define ADDR_PCA954X 0x70
#endif

#define BME280_ADDRESS   0x76

#define BME280_REG_DIG_T1    0x88
#define BME280_REG_DIG_T2    0x8A
#define BME280_REG_DIG_T3    0x8C

#define BME280_REG_DIG_P1    0x8E
#define BME280_REG_DIG_P2    0x90
#define BME280_REG_DIG_P3    0x92
#define BME280_REG_DIG_P4    0x94
#define BME280_REG_DIG_P5    0x96
#define BME280_REG_DIG_P6    0x98
#define BME280_REG_DIG_P7    0x9A
#define BME280_REG_DIG_P8    0x9C
#define BME280_REG_DIG_P9    0x9E

#define BME280_REG_DIG_H1    0xA1
#define BME280_REG_DIG_H2    0xE1
#define BME280_REG_DIG_H3    0xE3
#define BME280_REG_DIG_H4    0xE4
#define BME280_REG_DIG_H5    0xE5
#define BME280_REG_DIG_H6    0xE7

#define BME280_REG_CHIPID          0xD0
#define BME280_REG_VERSION         0xD1
#define BME280_REG_SOFTRESET       0xE0

#define BME280_REG_CAL26           0xE1

#define BME280_REG_CONTROLHUMID    0xF2
#define BME280_REG_CONTROL         0xF4
#define BME280_REG_CONFIG          0xF5
#define BME280_REG_PRESSUREDATA    0xF7
#define BME280_REG_TEMPDATA        0xFA
#define BME280_REG_HUMIDITYDATA    0xFD

/**
 * @class GroveI2C_BME280
 * @brief A class for interfacing with the BME280 sensor.
 *
 * This class allows reading temperature, pressure, and humidity data from the BME280 sensor.
 * It also provides functions to calculate altitude based on pressure readings.
 */
class GroveI2C_BME280{
	private:
		int _devAddr;
		bool isTransport_OK;
		// Calibration data
		uint16_t dig_T1;
		int16_t dig_T2;
		int16_t dig_T3;
		uint16_t dig_P1;
		int16_t dig_P2;
		int16_t dig_P3;
		int16_t dig_P4;
		int16_t dig_P5;
		int16_t dig_P6;
		int16_t dig_P7;
		int16_t dig_P8;
		int16_t dig_P9;
		uint8_t dig_H1;
		int16_t dig_H2;
		uint8_t dig_H3;
		int16_t dig_H4;
		int16_t dig_H5;
		int8_t  dig_H6;
		int32_t t_fine;
		
		// private functions
		uint8_t BME280Read8(uint8_t reg);
		uint16_t BME280Read16(uint8_t reg);
		uint16_t BME280Read16LE(uint8_t reg);
		int16_t BME280ReadS16(uint8_t reg);
		int16_t BME280ReadS16LE(uint8_t reg);
		uint32_t BME280Read24(uint8_t reg);
		void writeRegister(uint8_t reg, uint8_t val);

		void i2cMUXSelect();

	public:	
		uint8_t _ch=0;
		TwoWire* _pWire;

		/**
		 * @brief Initializes the BME280 sensor.
		 *
		 * @param i2c_addr The I2C address of the BME280 sensor (default is BME280_ADDRESS).
		 * @return true if initialization is successful, false otherwise.
		 */
		bool begin(int i2c_addr = BME280_ADDRESS);
		/**
		 * @brief Reads the temperature from the BME280 sensor.
		 *
		 * @return The temperature in degrees Celsius.
		 */
		float getTemperature(void);

		/**
		 * @brief Reads the pressure from the BME280 sensor.
		 *
		 * @return The pressure in hPa.
		 */
		float getPressure(void);

		/**
		 * @brief Reads the humidity from the BME280 sensor.
		 *
		 * @return The humidity as a percentage.
		 */
		float getHumidity(void);

		/**
		 * @brief Calculates the altitude based on the pressure reading.
		 *
		 * @param pressure The pressure in hPa.
		 * @return The calculated altitude in meters.
		 */
		float calcAltitude(float pressure);

};

#endif
