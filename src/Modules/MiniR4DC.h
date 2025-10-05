/**
 * @file MiniR4DC.h
 * @brief Handling MiniR4.Mn Motor functions.
 * @author MATRIX Robotics
 */

#ifndef MINIR4DC_H
#define MINIR4DC_H

#include "MMLower.h"

/**
 * @brief Class for controlling a DC motor with encoder functionality.
 *
 * This template class provides methods for initializing, controlling speed,
 * power, and direction of a DC motor, as well as for managing its encoder.
 *
 * @tparam ID The identifier for the motor.
 */
template<uint8_t ID> class MiniR4DC
{
public:
    MiniR4DC() { _id = ID; }

    /**
     * @brief Initializes the DC motor settings.
     *
     * This function sets the speed range, resets the encoder counter,
     * powers off the motor, and sets the direction to forward.
     *
     * Note: Once you call MiniR4.begin(), M1-M4 motor will automaticlly begin.
     * 
     * @return True if initialization was successful, false otherwise.
     */
    bool begin(void)
    {
        MMLower::RESULT result  = mmL.SetDCMotorSpeedRange(_id, 0, 100);
        MMLower::RESULT result1 = mmL.SetEncoderResetCounter(_id);
        MMLower::RESULT result2 = mmL.SetDCMotorPower(_id, 0);
        MMLower::RESULT result3 = mmL.SetDCMotorDir(_id, MMLower::DIR::FORWARD);

		//return (result2 == MMLower::RESULT::OK);

        return (
            result == MMLower::RESULT::OK && result1 == MMLower::RESULT::OK &&
            result2 == MMLower::RESULT::OK && result3 == MMLower::RESULT::OK);

    }
	
	/** 
	 * @brief Sets the encoder PPR (Pulses Per Revolution) and maximum RPM for a motor.
	 * 
	 * This function configures the encoder resolution and motor speed limits,
	 * which are used for accurate position and speed control in various drive functions.
	 * 
	 * @param motorPPR Encoder pulses per revolution (PPR).
	 * @param motorMaxRPM Motor maximum RPM (revolutions per minute).
	 * @return True if the PPR/MaxRPM parameters were successfully set, false otherwise.
	 */
	 bool setPPR_RPM(uint16_t motorPPR, uint16_t motorMaxRPM)
	{		
		MMLower::RESULT result = mmL.SetEncode_PPR_MaxRPM(_id, motorPPR, motorMaxRPM);		
		return (result == MMLower::RESULT::OK);
	}
	
    /**
     * @brief Sets the direction of the DC motor.
     * 
     * @param dir True to set direction to reverse, false for forward.
     * @return True if the direction was successfully set, false otherwise.
     */
    bool setReverse(bool dir)
    {
        MMLower::DIR    _dir        = (dir) ? MMLower::DIR::REVERSE : MMLower::DIR::FORWARD;
        MMLower::RESULT resultMotor = mmL.SetDCMotorDir(_id, _dir);
        // MMLower::RESULT resultEnc   = mmL.SetEncoderDir(_id, _dir);
        // return (resultMotor == MMLower::RESULT::OK && resultEnc == MMLower::RESULT::OK);
        return (resultMotor == MMLower::RESULT::OK);
    }

    /**
     * @brief Sets the power level of the DC motor.
     * 
     * It's like LEGO EV3/SPIKE Power block (Unregulated Motor)
     * It will output the Motor PWM from 0% to 100%
     * 
     * @param power The power level to set (-100 to 100).
     * @return True if the power was successfully set, false otherwise.
     */
    bool setPower(int16_t power)
    {
        MMLower::RESULT result = mmL.SetDCMotorPower(_id, power);
		
		if(result != MMLower::RESULT::OK){
			delay(1);
			result = mmL.SetDCMotorPower(_id, power);
		}
		
        return (result == MMLower::RESULT::OK);
    }

    /**
     * @brief Sets the speed of the DC motor.
     * 
     * It's like LEGO EV3/SPIKE Speed block (EV3 Green Motor)
     * The Lower MCU (STM32) will control the motor PWM to try keep the speed you set.
     * 
     * @param speed The speed to set (-100 to 100).
     * @return True if the speed was successfully set, false otherwise.
     */
    bool setSpeed(int16_t speed)
    {
        MMLower::RESULT result = mmL.SetDCMotorSpeed(_id, speed);
        return (result == MMLower::RESULT::OK);
    }

    /**
     * @brief Rotates the DC motor for a specific degree at a given speed.
     * 
     * It's like LEGO EV3/SPIKE Motor rotate for degress block.
     * Note: Non-Blocking function, you can use ChkRotateEnd() to check motor finish rotate or not.
     * Note: If motor not complete rotate and revice other command like setSpeed(), the rotate will be skip. 
     * 
     * @param speed The speed at which to rotate the motor.
     * @param degree The degree of rotation.
     * @return True if the rotation command was successful, false otherwise.
     */
    bool rotateFor(int16_t speed, uint16_t degree)
    {
        MMLower::RESULT result = mmL.SetDCMotorRotate(_id, speed, degree);
        return (result == MMLower::RESULT::OK);
    }

    /**
     * @brief Sets the PID parameters for fixed speed control. (For SetSpeed())
     *
     * @param kp Proportional gain.
     * @param ki Integral gain.
     * @param kd Derivative gain.
     * @return True if the PID parameters were successfully set, false otherwise.
     */
    bool setFixSpeedPID(float kp, float ki, float kd)
    {
        MMLower::RESULT result = mmL.SetPIDParam(_id, 0, kp, ki, kd);
        return (result == MMLower::RESULT::OK);
    }	

    /**
     * @brief Sets the PID parameters for rotation control. (For rotateFor())
     * 
     * @param kp Proportional gain.
     * @param ki Integral gain.
     * @param kd Derivative gain.
     * @return True if the PID parameters were successfully set, false otherwise.
     */
    bool setRotatePID(float kp, float ki, float kd)
    {
        MMLower::RESULT result = mmL.SetPIDParam(_id, 1, kp, ki, kd);
        return (result == MMLower::RESULT::OK);
    }

    /**
     * @brief Gets the current encoder counter value. (Not Degree)
     * 
     * @return The current encoder counter value.
     */
    int32_t getCounter(void)
    {
        int32_t         counter = 0;
        MMLower::RESULT result  = mmL.GetEncoderCounter(_id, counter);
        return counter;
    }
	


     /**
     * @brief Gets the current rotation in degrees based on the encoder count.
     * 
     * @return The calculated degrees of rotation.
     */
    int32_t getDegrees(void)
    {
        int32_t         degs = 0;
        MMLower::RESULT result  = mmL.GetEncoderDegrees(_id, degs);
        return degs;
    }

    /**
     * @brief Resets the encoder counter to zero.
     * 
     * @return True if the counter was successfully reset, false otherwise.
     */
    bool resetCounter(void)
    {
        MMLower::RESULT result = mmL.SetEncoderResetCounter(_id);
        return (result == MMLower::RESULT::OK);
    }

    /**
     * @brief Sets the brake mode for the DC motor.
     * 
     * Like LEGO EV3/SPIKE, True is Brake, False is Coast.
     * 
     * @param brake True to engage the brake, false to stop the motor.
     * @return True if the brake was successfully engaged, false otherwise.
     */
    bool setBrake(bool brake)
    {
        if (brake) {
            MMLower::RESULT result = mmL.SetDCBrake(_id);
            return (result == MMLower::RESULT::OK);
        } else {
            MMLower::RESULT result = mmL.SetDCMotorSpeed(_id, 0);
            return (result == MMLower::RESULT::OK);
        }
    }

    /**
     * @brief Checks if the rotation has ended.
     * 
     * @param isEnd Reference to a boolean that will be set to true if rotation has ended, false otherwise.
     * @return True if the state was successfully retrieved, false otherwise.
     */
    bool ChkRotateEnd(bool& isEnd)
    {
        MMLower::RESULT result = mmL.GetRotateState(_id, isEnd);
        return (result == MMLower::RESULT::OK);
    }

private:
    uint8_t _id;
};

#endif   // MINIR4DC_H
