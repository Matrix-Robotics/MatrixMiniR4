/**
 * @file MiniR4_EIOBoardExt.h
 * @brief Handling Matrix Erobot I/O Board functions.
 * @author MATRIX Robotics
 */
#ifndef _MiniR4_EIOBoardExt_H_
#define _MiniR4_EIOBoardExt_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#    define ADDR_PCA954X 0x70
#endif

#define MatrixEIOBoard_ADDR 	0x01

/**
 * @brief Class for controlling E.ROBOT I/O Board (NXT).
 *
 * E.ROBOT I/O Board is a simple IO controller board for NXT, manufacturer by E.ROBOT.
 * 
 * Currently only suppurt digital out mode.
 */
class MatrixEIOBoard{
	private:
		uint8_t _ver=1;

		void i2cMUXSelect();
		void i2cWriteData(uint8_t action, uint8_t range, uint8_t pwm1, uint8_t pwm2);

	public:	
		uint8_t  _ch = 0;
		TwoWire* _pWire;

		/**
		 * @brief Sets the high low states of digital pin.
		 *
		 * @param 0~255 to controll pins high low, ex. 0b11111111(0xff, 255), means all pin set to high.
		 *
		 */
		void digitalOut(uint8_t pins);
		
		// /**
		 // * @brief Sets the pwm of pwm pin.
		 // *
		 // * @param 0-255 to controll duty of pwm pin 1.
		 // * @param 0-255 to controll duty of pwm pin 2.
		 // * 
		 // */
		// void pwmOut(uint8_t pwm1, uint8_t pwm2);
};

#endif
