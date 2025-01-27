/**
 * @file MiniR4MxCtrlExt.h
 * @brief Handling Matrix Controller (HT) functions.
 * @author MATRIX Robotics
 */
#ifndef _MiniR4MxCtrlExt_H_
#define _MiniR4MxCtrlExt_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#    define ADDR_PCA954X 0x70
#endif

#define MatrixCtrl_ADDR 	0x08

#define MC_REG_STATUS		0x41
#define MC_REG_TIMEOUT		0x42
#define MC_REG_BATTERY		0x43
#define MC_REG_STARTFLAG	0x44

#define MC_REG_SERVO_ENABLE	0x45	
#define MC_REG_SERVO_SPEED	0x46
#define MC_REG_SERVO_TARGET	0x47

#define MC_REG_SERVO_OFFSET 2

#define MC_REG_MOTOR_POS	0x4E	
#define MC_REG_MOTOR_TARGET	0x52	
#define MC_REG_MOTOR_SPEED	0x56
#define MC_REG_MOTOR_MODE	0x57

#define MC_REG_MOTOR_OFFSET	10

#define MODE_BRAKE			1
#define MODE_POSITION		0

/**
 * @brief Class for controlling motors and servos in the Matrix Controller (HT).
 *
 * Matrix Controller is a blue box can control 9v motor, manufacturer by HiTechnic.
 * 
 * This class provides methods for setting motor speed, target position,
 * servo angles, and reading battery status.
 */
class MatrixController{
	private:
		uint8_t _ver=1;
		uint8_t _invert1, _invert2, _invert3, _invert4;
		void motorMode(uint8_t motor, uint8_t mode);
		uint32_t motorPosition(uint8_t);

		void i2cMUXSelect();
		void i2cWriteData(uint8_t reg, uint8_t data);
		uint32_t i2cReadData(int reg, int num);

	public:	
		uint8_t _ch=0;
		TwoWire* _pWire;

		/**
		 * @brief Initializes the MatrixController.
		 */
		void begin();

		/**
		 * @brief Sets the speed of the specified motor.
		 *
		 * @param motor_ch The channel of the motor to set.
		 * @param speed The speed to set for the motor.
		 */
		void motorSet(uint8_t motor_ch, uint8_t speed);

		/**
		 * @brief Moves the specified motor a certain number of rounds.
		 *
		 * @param motor_ch The channel of the motor to move.
		 * @param round The number of rounds to move the motor.
		 */
		void motorRound(uint8_t motor_ch, float round);

		/**
		 * @brief Inverts the direction of the specified motor.
		 *
		 * @param motor_ch The channel of the motor to invert.
		 * @param invert True to invert, false to keep normal direction.
		 */
		void motorInvert(uint8_t motor_ch, bool invert);

		/**
		 * @brief Sets the angle of the specified servo.
		 *
		 * @param servo_ch The channel of the servo to set.
		 * @param angle The angle to set for the servo.
		 */
		void servoSet(uint8_t servo_ch, uint8_t angle);

		/**
		 * @brief Gets the current battery status.
		 *
		 * @return The battery voltage in volts.
		 */
		float getBattery();
};

#endif
