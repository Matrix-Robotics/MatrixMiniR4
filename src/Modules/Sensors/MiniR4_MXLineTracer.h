/**
 * @file MiniR4_MXLineTracer.h
 * @brief A library for interfacing with the MATRIX Line Tracer 10CH sensor via I2C.
 * 
 * @author MATRIX Robotics
 * @version 1.1
 * @date 2025
 * @license MIT License
 */
#ifndef _MiniR4_MXLineTracer_H_
#define _MiniR4_MXLineTracer_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#    define ADDR_PCA954X 0x70
#endif

// Sensor I2C Addr
#define MXLINE_ADDRESS 0x30

/**
 * @class MatrixLineTracer
 * @brief A class for MATRIX Line Tracer 10CH sensor.
 */
class MatrixLineTracer{
	private:
		uint8_t _address;
		uint8_t _sensorCache[10];
		float _sensorWeights[10];
		uint8_t _threshold;
		uint8_t read(byte cmd);
		void i2cMUXSelect();
	
	public:
		uint8_t _ch = 0; ///< I2C multiplexer channel
		TwoWire* _pWire; ///< Pointer to Wire object
		
		/**
		 * @brief Initialize the LineTracer sensor
		 */
		void begin();
  
		/**
		 * @brief Get All sensor value into array.
		 * @param data The array need to storage value.
		 */
		bool getAllSensors(uint8_t data[10]);
		
		/**
		 * @brief Get single sensor value.
		 * @param n Sensor ID (1~10).
		 * @return Light value.
		 */
		uint8_t getSensor(uint8_t n);
		
		/**
		 * @brief Get sensor firmware version.
		 * @return Version String.
		 */
		String getVersion();
		
		/**
		 * @brief Get Line Width.
		 * @return How many sensor online (1~10).
		 */
		uint8_t getLineWidth();
		// int8_t getError();
		
		/**
		 * @brief Get Error (Weights based).
		 * @return Error value (Default: -4.5 ~ 4.5).
		 */
		float getError();
		
		/**
		 * @brief Is Sensor still online?
		 * @return True or False.
		 */
		bool isOnline();
		
		/**
		 * @brief Get last sensor online before robot go offline.
		 * @return Sensor ID (1~10).
		 */
		uint8_t getLastSensor();
		
		/**
		 * @brief Get Junction Type.
		 * @return 0 = None, 1 = Left, 2 = Right, 3 = T or Cross, 4 = Unknown/None
		 */
		uint8_t getJunctionType();
		
		/**
		 * @brief Set Weights for error calc.
		 * @param w1 weights for S1
		 * @param w2 weights for S2
		 * @param w3 weights for S3
		 * @param w4 weights for S4
		 * @param w5 weights for S5
		 * @param w6 weights for S6
		 * @param w7 weights for S7
		 * @param w8 weights for S8
		 * @param w9 weights for S9
		 * @param w10 weights for S10
		 */
		void setWeights(float w1, float w2, float w3, float w4, float w5, 
		                float w6, float w7, float w8, float w9, float w10);
						
		/**
		 * @brief Set threshold for error calc.
		 * @param threshold Threshold for line.
		 */
		void setThreshold(uint8_t threshold);
		
		/**
		 * @brief Start Calibration Mode
		 */
		void startCalibration();
		
		/**
		 * @brief End Calibration Mode
		 */
		void endCalibration();
};

#endif