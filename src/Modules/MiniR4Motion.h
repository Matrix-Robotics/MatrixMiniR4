/**
 * @file MiniR4Motion.h
 * @brief Handling Built-in IMU functions.
 * @author MATRIX Robotics
 */
#ifndef MINIR4MOTION_H
#define MINIR4MOTION_H

#include "MMLower.h"
#include <EEPROM.h>

/**
 * @brief Class for motion sensing using an IMU (Inertial Measurement Unit).
 *
 * Note: This Class is for Controller built-in IMU (PU6050) control by Lower MCU (STM32)
 * 
 * This class provides methods to retrieve gyro, accelerometer, and Euler angles 
 * data from the IMU. It also allows resetting the IMU values to zero.
 */
class MiniR4Motion
{
public:
    MiniR4Motion() {}

    enum class AxisType
    {
        X,
        Y,
        Z,
        Roll,
        Pitch,
        Yaw
    };
	
    bool begin(void)
    {
		bool result = applyIMUCalData();
        return (result);
    }	

    /**
     * @brief Gets the gyro value for a specified axis.
     *
     * @param axis The axis for which to retrieve the gyro value (X, Y, Z).
     * @return The gyro value for the specified axis.
     */
    double getGyro(AxisType axis)
    {
        double x = 0, y = 0, z = 0;
        mmL.GetIMUGyro(x, y, z);

        if (axis == AxisType::X)
            return x;
        else if (axis == AxisType::Y)
            return y;
        else if (axis == AxisType::Z)
            return z;
        else
            return 0;
    }

    /**
     * @brief Gets the accelerometer value for a specified axis.
     *
     * @param axis The axis for which to retrieve the accelerometer value (X, Y, Z).
     * @return The accelerometer value for the specified axis.
     */
    double getAccel(AxisType axis)
    {
        double x = 0, y = 0, z = 0;
        mmL.GetIMUAcc(x, y, z);

        if (axis == AxisType::X)
            return x;
        else if (axis == AxisType::Y)
            return y;
        else if (axis == AxisType::Z)
            return z;
        else
            return 0;
    }
	
	/**
	 * @brief Gets the raw (uncalibrated) IMU accelerometer data for a specific axis
	 * 
	 * Reads the uncalibrated accelerometer value for the specified axis (X, Y, or Z).
	 * 
	 * @param axis The axis to read (AxisType::X, AxisType::Y, or AxisType::Z)
	 * @return The raw accelerometer value for the specified axis, or 0 if invalid axis
	 */
	double getAccelRaw(AxisType axis)
	{
		float accdataCX[3];
		bool result = getIMU_acc_real(accdataCX);
		
		if (!result)
			return 0;
		
		if (axis == AxisType::X)
			return accdataCX[0];
		else if (axis == AxisType::Y)
			return accdataCX[1];
		else if (axis == AxisType::Z)
			return accdataCX[2];
		else
			return 0;
	}
	
    /**
     * @brief Gets the Euler angle for a specified axis.
     *
     * @param axis The axis for which to retrieve the Euler angle (Roll, Pitch, Yaw).
     * @return The Euler angle for the specified axis.
     */
    double getEuler(AxisType axis)
    {
        double roll = 0, pitch = 0, yaw = 0;
        mmL.GetIMUEuler(roll, pitch, yaw);

        if (axis == AxisType::Roll)
            return roll;
        else if (axis == AxisType::Pitch)
            return pitch;
        else if (axis == AxisType::Yaw)
            return yaw;
        else
            return -999.0f;
    }	
	
    /**
     * @brief Gets the accelerometer value for a specified axis.
     *
     * @param axis The axis for which to retrieve the accelerometer value (X, Y, Z).
     * @return The accelerometer value for the specified axis.
     */
    uint8_t getAccel_All(double * dataX)
    {
        double x = 0, y = 0, z = 0;
        mmL.GetIMUAcc(x, y, z);

		dataX[0] = x;
		dataX[1] = y;
		dataX[2] = z;
		
        return 0;
    }	
	
	    /**
     * @brief Gets the gyro value for a specified axis.
     *
     * @param axis The axis for which to retrieve the gyro value (X, Y, Z).
     * @return The gyro value for the specified axis.
     */
    uint8_t getGyro_All(double * dataX)
    {
        double x = 0, y = 0, z = 0;
        mmL.GetIMUGyro(x, y, z);
		dataX[0] = x;
		dataX[1] = y;
		dataX[2] = z;
		
        return 0;
    }

    /**
     * @brief Gets the Euler angle for a specified axis.
     *
     * @param axis The axis for which to retrieve the Euler angle (Roll, Pitch, Yaw).
     * @return The Euler angle for the specified axis.
     */
    uint8_t getEuler_All(double * dataX)
    {
        double roll = 0, pitch = 0, yaw = 0;
        mmL.GetIMUEuler(roll, pitch, yaw);

		dataX[0] = roll;
		dataX[1] = pitch;
		dataX[2] = yaw;
		
        return 0;
    }
	
	/**
	 * @brief Save IMU calibration data with 6 individual parameters (R4 EEPROM 0x32 - 0x49)
	 * 
	 * @param face1 Calibration value for face 1 
	 * @param face2 Calibration value for face 2 
	 * @param face3 Calibration value for face 3 
	 * @param face4 Calibration value for face 4 
	 * @param face5 Calibration value for face 5 
	 * @param face6 Calibration value for face 6 
	 * @return true if calibration data was successfully saved, false otherwise
	 */
	bool saveIMUCalData(float face1, float face2, float face3, float face4, float face5, float face6)
	{
		float accdata[6] = {face1, face2, face3, face4, face5, face6};
		
		int Address_F = 50;
		EEPROM.put(Address_F, accdata[0]);
		Address_F += sizeof(float);
		EEPROM.put(Address_F, accdata[1]);
		Address_F += sizeof(float);
		EEPROM.put(Address_F, accdata[2]);
		Address_F += sizeof(float);
		EEPROM.put(Address_F, accdata[3]);
		Address_F += sizeof(float);
		EEPROM.put(Address_F, accdata[4]);
		Address_F += sizeof(float);
		EEPROM.put(Address_F, accdata[5]);
		
		MMLower::RESULT result = mmL.SetIMU_Calib_data(accdata);
		return (result == MMLower::RESULT::OK);
	}

	/**
	 * @brief Load and send IMU calibration data from EEPROM (R4 EEPROM 0x32 - 0x49)
	 *
	 * Reads the Six-Position Calibration data from EEPROM and sends it to the STM32 unit.
	 *
	 * @return true if calibration data was successfully loaded and sent, false otherwise
	 */
	bool applyIMUCalData(void)
	{		
		float accdata[6];
		
		int Address_F = 50;		
		EEPROM.get(Address_F, accdata[0]);
		Address_F += sizeof(float);
		EEPROM.get(Address_F, accdata[1]);
		Address_F += sizeof(float);
		EEPROM.get(Address_F, accdata[2]);	
		Address_F += sizeof(float);
		EEPROM.get(Address_F, accdata[3]);	
		Address_F += sizeof(float);
		EEPROM.get(Address_F, accdata[4]);	
		Address_F += sizeof(float);
		EEPROM.get(Address_F, accdata[5]);	
		
		MMLower::RESULT result = mmL.SetIMU_Calib_data(accdata);
		return (result == MMLower::RESULT::OK);
	}
	
    /**
     * @brief Gets the current encoder Speed value. (RPS)
     * 
     * @return True if the Value is get successfully, false otherwise.
     */
    bool getAllSpeed(int32_t * Speed_value)
    {
        int32_t         Speed_En[4];
        MMLower::RESULT result  = mmL.GetALLEncoderSpeed(Speed_En);
		delay(1);
		result  = mmL.GetALLEncoderSpeed(Speed_En);
		Speed_value[0] = Speed_En[0];
		Speed_value[1] = Speed_En[1];
		Speed_value[2] = Speed_En[2];
		Speed_value[3] = Speed_En[3];
		
        return (result == MMLower::RESULT::OK);
    }	
	
    /**
	 * @brief Gets the raw (uncalibrated) IMU accelerometer data
	 * 
	 * Reads the 3-axis accelerometer values without calibration applied.
	 * Performs two consecutive reads with a 2ms delay for stability.
	 * 
	 * @param accdataCX Pointer to float array to store the accelerometer data (X, Y, Z axes)
	 * @return true if data was successfully retrieved, false otherwise
	 */
	bool getIMU_acc_real(float * accdataCX)
	{
		float         Speed_En[4];
		MMLower::RESULT result  = mmL.Get_IMU_nancalib_acc(Speed_En);
		delay(2);
		result  = mmL.Get_IMU_nancalib_acc(Speed_En);
		
		accdataCX[0] = Speed_En[0];
		accdataCX[1] = Speed_En[1];
		accdataCX[2] = Speed_En[2];
		
		return (result == MMLower::RESULT::OK);
	}	

    /**
     * @brief Resets the IMU values to zero.
     *
     * @return True if the reset operation was successful, false otherwise.
     */
    bool resetIMUValues(void) { return (mmL.SetIMUToZero() == MMLower::RESULT::OK); }

private:
};

#endif   // MINIR4MOTION_H
