/**
 * @file MiniR4DriveDC.h
 * @brief Handling MiniR4.DriveDC functions.
 * @author MATRIX Robotics
 */

#ifndef MINIR4DRIVEDC_H
#define MINIR4DRIVEDC_H

#include "MMLower.h"

#define IMU_idle_Period 30
#define Drive_retry_Period 10

#ifdef ENABLE_DRIVEDC_BRAKE_DELAY
    #ifndef DRIVEDC_BRAKE_DELAY_MS
        #define DRIVEDC_BRAKE_DELAY_MS 110
    #endif
#endif

/**
 * @brief Class for controlling a DC motor with encoder functionality.
 *
 * This template class provides methods for initializing, driving DC motor by Move,
 * MoveSync, MoveGyro, TurnGyro function
 *
 * @tparam ID The identifier for the motor.
 */
template < uint8_t ID > class MiniR4DriveDC {
  public: MiniR4DriveDC() {
    _id = ID;
  }

  /**
   * @brief Initializes the DC motor settings.
   *
   * This function sets the speed range, resets the encoder counter,
   * powers off the motor, and sets the direction to forward.
   *
   * Note: Once you call MiniR4.DriveDC.begin, DriveDC class motor will automatically begin.
   *
   * @param	leftMotorID : the number of Left motor
   * @param	rightMotorID : the number of Right motor
   * @param	isLeftReverse : the direction of Left motor encoder
   * @param isRightReverse : the direction of Right motor encoder
   * 
   * @return 0x00 if initialization was successful, error code otherwise.
   */
  uint8_t begin(uint8_t leftMotorID, uint8_t rightMotorID, bool isLeftReverse, bool isRightReverse) {
    _id_left = leftMotorID;
    _id_right = rightMotorID;
    _dir_left = (isLeftReverse) ? MMLower::DIR::REVERSE : MMLower::DIR::FORWARD;
    _dir_right = (isRightReverse) ? MMLower::DIR::REVERSE : MMLower::DIR::FORWARD;
    uint8_t idx = 0;

    MMLower::Drive_RESULT result = mmL.Set_Drive2Motor_PARAM(_id_left, _id_right, _dir_left, _dir_right, _id);

    while (result != MMLower::Drive_RESULT::OK) {
      delay(100);
      result = mmL.Set_Drive2Motor_PARAM(_id_left, _id_right, _dir_left, _dir_right, _id);
      if (result == MMLower::Drive_RESULT::ERROR_Drive_Param) {
        idx++;
        if (idx > IMU_idle_Period) {
          return 0x08;
        }
      } else if (result == MMLower::Drive_RESULT::OK) {
        return 0x00;
      } else if (result == MMLower::Drive_RESULT::ERROR_Drive_Param) {
        idx++;
        if (idx > IMU_idle_Period) {
          return 0x09;
        }
      }
    }

    return 0x00;

  }

  /**
   * @brief Sets the DriveDC motor parameters (Unuse)
   * 
   * @param	m1_no : the number of Left motor
   * @param	m2_no : the number of Right motor
   * @param	m1_dir : the direction of Left motor encoder
   * @param m2_dir : the direction of Right motor encoder	 
   * @return 0x00 if the parameters were successfully set, error code otherwise.
   */
  // uint8_t set_Drive_Param(uint8_t m1_no, uint8_t m2_no, bool m1_dir, bool m2_dir) {
    // _id_left = m1_no;
    // _id_right = m2_no;
    // _dir_left = (m1_dir) ? MMLower::DIR::REVERSE : MMLower::DIR::FORWARD;
    // _dir_right = (m2_dir) ? MMLower::DIR::REVERSE : MMLower::DIR::FORWARD;
    // uint8_t idx = 0;

    // MMLower::Drive_RESULT result = mmL.Set_Drive2Motor_PARAM(_id_left, _id_right, _dir_left, _dir_right, _id);

    // while (result != MMLower::Drive_RESULT::OK) {
      // delay(100);
      // result = mmL.Set_Drive2Motor_PARAM(_id_left, _id_right, _dir_left, _dir_right, _id);
      // if (result == MMLower::Drive_RESULT::ERROR_Drive_Param) {
        // idx++;
        // if (idx > IMU_idle_Period) {
          // return 0x08;
        // }
      // } else if (result == MMLower::Drive_RESULT::OK) {
        // return 0x00;
      // } else if (result == MMLower::Drive_RESULT::ERROR_Drive_Param) {
        // idx++;
        // if (idx > IMU_idle_Period) {
          // return 0x09;
        // }
      // }
    // }

    // return 0x00;
  // }

  /**
   * @brief Sets the PID parameters for DriveDC MoveSync.
   * 
   * @param kp Proportional gain.(TT:0.02, LE:0.002)
   * @param ki Integral gain.(TT:0.00, LE:0.00)
   * @param kd Derivative gain.(TT:0.04, LE:0.004)
   * @return True if the PID parameters were successfully set, false otherwise.
   */
  bool setMoveSyncPID(float kp, float ki, float kd) {
    MMLower::RESULT result = mmL.Set_Drive_MoveSync_PID(kp, ki, kd, _id);

    if (result != MMLower::RESULT::OK) {
      delay(2);
      result = mmL.Set_Drive_MoveSync_PID(kp, ki, kd, _id);
    }

    return (result == MMLower::RESULT::OK);
  }

  /**
   * @brief Sets the PID parameters for DriveDC MoveGyro.
   * 
   * @param kp Proportional gain.(TT:6.01, LE:0.52)
   * @param ki Integral gain.(TT:0.00, LE:0.00)
   * @param kd Derivative gain.(TT:2.15, LE:0.15)
   * @return True if the PID parameters were successfully set, false otherwise.
   */
  bool setMoveGyroPID(float kp, float ki, float kd) {
    MMLower::RESULT result = mmL.Set_Drive_MoveGyro_PID(kp, ki, kd, _id);

    if (result != MMLower::RESULT::OK) {
      delay(2);
      result = mmL.Set_Drive_MoveGyro_PID(kp, ki, kd, _id);
    }

    return (result == MMLower::RESULT::OK);
  }

  /**
   * @brief Sets the PID parameters for DriveDC TurnGyro.
   * 
   * @param kp Proportional gain.(TT:22.75, LE-F:22.75, LE-S:5.25)
   * @param ki Integral gain.(TT:0.08, LE:0.02)
   * @param kd Derivative gain.(TT:0.25, LE:1.54)
   * @return True if the PID parameters were successfully set, false otherwise.
   */
  bool setTurnGyroPID(float kp, float ki, float kd) {
    MMLower::RESULT result = mmL.Set_Drive_MoveTurn_PID(kp, ki, kd, _id);

    if (result != MMLower::RESULT::OK) {
      delay(2);
      result = mmL.Set_Drive_MoveTurn_PID(kp, ki, kd, _id);
    }

    return (result == MMLower::RESULT::OK);
  }

  /**
   * @brief Sets the Motor Type for DriveDC. (Unuse)
   * 
   * @param typeXC => 1:Blue Motor / 2:LEGO Motor
   * @return True if the motor type was successfully set, false otherwise.
   */
  // bool setMotorType(uint8_t typeXC) {
    // MMLower::Drive_RESULT result = mmL.Set_Drive_Motor_Type(_id, typeXC);

    // if (result != MMLower::Drive_RESULT::OK) {
      // delay(2);
      // result = mmL.Set_Drive_Motor_Type(_id, typeXC);
    // }

    // return (result == MMLower::Drive_RESULT::OK);
  // }

  /**
   * @brief Sets the power level of the DC motor.
   * 
   * It's like LEGO EV3/SPIKE Power block (Unregulated Motor)
   * It will output the Motor PWM from 0% to 100%
   * 
   * @param power_left The power level to set for left motor (-100 to 100).
   * @param power_right The power level to set for right motor (-100 to 100).
   * @return 0x00 if the power was successfully set, error code otherwise.
   */
  uint8_t Move(int16_t power_left, int16_t power_right) {

    MMLower::Drive_RESULT result = mmL.Set_Drive_Move_Func(power_left, power_right, _id);

    uint8_t idx = 0;
    while (result != MMLower::Drive_RESULT::OK) {
      delay(2);
      result = mmL.Set_Drive_Move_Func(power_left, power_right, _id);
      idx++;
      if (result == MMLower::Drive_RESULT::ERROR_Drive_Define) {
        if (idx >= Drive_retry_Period)
          return 0x07;
      } else if (result != MMLower::Drive_RESULT::OK) {
        if (idx >= Drive_retry_Period)
          return 0x01;
      } else {
        return 0x00;
      }
    }
  }

  /**
   * @brief Sets the power level of the DC motor for a specific degree.
   * 
   * It's like LEGO EV3/SPIKE Power block (Unregulated Motor)
   * It will output the Motor PWM from 0% to 100%
   * 
   * @param power_left The power level to set for left motor (-100 to 100).
   * @param power_right The power level to set for right motor (-100 to 100).
   * @param degree Target Degree for Stop
   * @param brake True(Brake) / false(coast)
   * @param async True(non-block) / false(block) - default: false
   * @return 0x00 if the param was successfully set, error code otherwise.
   */
  uint8_t MoveDegs(int16_t power_left, int16_t power_right, uint16_t degree, bool brake, bool async = false) {
    MMLower::Drive_RESULT result = mmL.Set_Drive_Move_Degs(power_left, power_right, degree, brake, async, _id);

    uint8_t idx = 0;
    while (result != MMLower::Drive_RESULT::OK) {
      delay(2);
      result = mmL.Set_Drive_Move_Degs(power_left, power_right, degree, brake, async, _id);
      idx++;
      if (result == MMLower::Drive_RESULT::ERROR_Drive_Define) {
        if (idx >= Drive_retry_Period)
          return 0x07;
      } else if (result != MMLower::Drive_RESULT::OK) {
        if (idx >= Drive_retry_Period)
          return 0x01;
      } else {
        break;
      }
    }

    if (async == false) {
      bool stats[2];
      stats[0] = true;
      while (stats[0]) {
        delay(50);
        mmL.Get_Drive_isTaskDone(_id, stats);
      }
	  
      #ifdef ENABLE_DRIVEDC_BRAKE_DELAY
      delay(DRIVEDC_BRAKE_DELAY_MS); //Give motor some time to stop.
      #endif
    }
    return 0x00;
  }

  /**
   * @brief Sets the power level of the DC motor for a specific time period.
   * 
   * It's like LEGO EV3/SPIKE Power block (Unregulated Motor)
   * It will output the Motor PWM from 0% to 100%
   * 
   * @param power_left The power level to set for left motor (-100 to 100).
   * @param power_right The power level to set for right motor (-100 to 100).
   * @param Time_S Period of the Motor Running (secs)
   * @param brake True(Brake) / false(coast)
   * @param async True(non-block) / false(block) - default: false
   * @return 0x00 if the param was successfully set, error code otherwise.
   */
  uint8_t MoveTime(int16_t power_left, int16_t power_right, float Time_S, bool brake, bool async = false) {
    Time_S = Time_S * 1000; // to milliseconds
	MMLower::Drive_RESULT result = mmL.Set_Drive_Move_Time(power_left, power_right, Time_S, brake, async, _id);

    uint8_t idx = 0;
    while (result != MMLower::Drive_RESULT::OK) {
      delay(2);
      result = mmL.Set_Drive_Move_Time(power_left, power_right, Time_S, brake, async, _id);
      idx++;
      if (result == MMLower::Drive_RESULT::ERROR_Drive_Define) {
        if (idx >= Drive_retry_Period)
          return 0x07;
      } else if (result != MMLower::Drive_RESULT::OK) {
        if (idx >= Drive_retry_Period)
          return 0x01;
      } else {
        break;
      }
    }

    if (async == false) {
      bool stats[2];
      stats[0] = true;
      while (stats[0]) {
        delay(50);
        mmL.Get_Drive_isTaskDone(_id, stats);
      }
	  
	  #ifdef ENABLE_DRIVEDC_BRAKE_DELAY
      delay(DRIVEDC_BRAKE_DELAY_MS); //Give motor some time to stop.
      #endif
    }
    return 0x00;
  }

  /**
   * @brief Sets the power level of the DC motor with synchronization.
   * 
   * It's like LEGO EV3/SPIKE Power block (Regulated Motor)
   * Motors will run synchronized with encoder feedback
   * 
   * @param power_left The power level to set for left motor (-100 to 100).
   * @param power_right The power level to set for right motor (-100 to 100).
   * @return 0x00 if the power was successfully set, error code otherwise.
   */
  uint8_t MoveSync(int16_t power_left, int16_t power_right) {
    MMLower::Drive_RESULT result = mmL.Set_Drive_MoveSync_Func(power_left, power_right, _id);

    uint8_t idx = 0;
    while (result != MMLower::Drive_RESULT::OK) {
      delay(2);
      result = mmL.Set_Drive_MoveSync_Func(power_left, power_right, _id);
      idx++;
      if (result == MMLower::Drive_RESULT::ERROR_Drive_Define) {
        if (idx >= Drive_retry_Period)
          return 0x07;
      } else if (result != MMLower::Drive_RESULT::OK) {
        if (idx >= Drive_retry_Period)
          return 0x01;
      } else {
        return 0x00;
      }

    }
  }

  /**
   * @brief Sets the power level of the DC motor with synchronization for specific degrees.
   * 
   * It's like LEGO EV3/SPIKE Power block (Regulated Motor)
   * Motors will run synchronized with encoder feedback
   * 
   * @param power_left The power level to set for left motor (-100 to 100).
   * @param power_right The power level to set for right motor (-100 to 100).
   * @param degree Target Degree
   * @param brake True(Brake) / false(coast)
   * @param async True(non-block) / false(block) - default: false
   * @return 0x00 if the param was successfully set, error code otherwise.
   */
  uint8_t MoveSyncDegs(int16_t power_left, int16_t power_right, uint16_t degree, bool brake, bool async = false) {
    MMLower::Drive_RESULT result = mmL.Set_Drive_MoveSync_Degs(power_left, power_right, degree, brake, async, _id);

    uint8_t idx = 0;
    while (result != MMLower::Drive_RESULT::OK) {
      delay(2);
      result = mmL.Set_Drive_MoveSync_Degs(power_left, power_right, degree, brake, async, _id);
      idx++;
      if (result == MMLower::Drive_RESULT::ERROR_Drive_Define) {
        if (idx >= Drive_retry_Period)
          return 0x07;
      } else if (result != MMLower::Drive_RESULT::OK) {
        if (idx >= Drive_retry_Period)
          return 0x01;
      } else {
        break;
      }
    }

    if (async == false) {
      bool stats[2];
      stats[0] = true;
      while (stats[0]) {
        delay(10);
        mmL.Get_Drive_isTaskDone(_id, stats);
      }
	  
	  #ifdef ENABLE_DRIVEDC_BRAKE_DELAY
      delay(DRIVEDC_BRAKE_DELAY_MS); //Give motor some time to stop.
      #endif
    }

    return 0x00;

  }

  /**
   * @brief Sets the power level of the DC motor with synchronization for specific time.
   * 
   * It's like LEGO EV3/SPIKE Power block (Regulated Motor)
   * Motors will run synchronized with encoder feedback
   * 
   * @param power_left The power level to set for left motor (-100 to 100).
   * @param power_right The power level to set for right motor (-100 to 100).
   * @param Time_S Period of the Motor Running (secs)
   * @param brake True(Brake) / false(coast)
   * @param async True(non-block) / false(block) - default: false
   * @return 0x00 if the param was successfully set, error code otherwise.
   */
  uint8_t MoveSyncTime(int16_t power_left, int16_t power_right, float Time_S, bool brake, bool async = false) {
    Time_S = Time_S * 1000; // to milliseconds
	MMLower::Drive_RESULT result = mmL.Set_Drive_MoveSync_Time(power_left, power_right, Time_S, brake, async, _id);

    uint8_t idx = 0;
    while (result != MMLower::Drive_RESULT::OK) {
      delay(2);
      result = mmL.Set_Drive_MoveSync_Time(power_left, power_right, Time_S, brake, async, _id);
      idx++;
      if (result == MMLower::Drive_RESULT::ERROR_Drive_Define) {
        if (idx >= Drive_retry_Period)
          return 0x07;
      } else if (result != MMLower::Drive_RESULT::OK) {
        if (idx >= Drive_retry_Period)
          return 0x01;
      } else {
        break;
      }
    }

    if (async == false) {
      bool stats[2];
      stats[0] = true;
      while (stats[0]) {
        delay(10);
        mmL.Get_Drive_isTaskDone(_id, stats);
      }
	  
	  #ifdef ENABLE_DRIVEDC_BRAKE_DELAY
      delay(DRIVEDC_BRAKE_DELAY_MS); //Give motor some time to stop.
      #endif
    }

    return 0x00;

  }

  /**
   * @brief Sets the power level with gyro guidance.
   * 
   * Robot moves with gyro sensor feedback to maintain straight line or target heading
   * 
   * @param power The power level to set (-100 to 100).
   * @param Target Target Degree of the IMU Yaw (-360 ~ +360)
   * @return 0x00 if the param was successfully set, error code otherwise.
   */
  uint8_t MoveGyro(int16_t power, int16_t Target) {
    MMLower::Drive_RESULT result = mmL.Set_Drive_MoveGyro_Func(power, Target, _id);

    uint8_t idx = 0;
    while (result != MMLower::Drive_RESULT::OK) {
      delay(2);
      result = mmL.Set_Drive_MoveGyro_Func(power, Target, _id);
      idx++;
      if (result == MMLower::Drive_RESULT::ERROR_Drive_Define) {
        if (idx >= Drive_retry_Period)
          return 0x07;
      } else if (result != MMLower::Drive_RESULT::OK) {
        if (idx >= Drive_retry_Period)
          return 0x01;
      } else {
        return 0x00;
      }

    }
  }

  /**
   * @brief Sets the power level with gyro guidance for specific degrees.
   * 
   * Robot moves with gyro sensor feedback to maintain heading while moving specific encoder degrees
   * 
   * @param power_X The power level to set (-100 to 100).
   * @param Target_D Target Degree of the IMU Yaw (-360 ~ +360)
   * @param degree Target Degree of the motor encoder for stop
   * @param brake True(Brake) / false(coast)
   * @param async True(non-block) / false(block) - default: false
   * @return 0x00 if the param was successfully set, error code otherwise.
   */
  uint8_t MoveGyroDegs(int16_t power_X, int16_t Target_D, uint16_t degree, bool brake, bool async = false) {
    MMLower::Drive_RESULT result = mmL.Set_Drive_MoveGyro_Degs(power_X, Target_D, degree, brake, async, _id);

    uint8_t idx = 0;
    while (result != MMLower::Drive_RESULT::OK) {
      delay(2);
      result = mmL.Set_Drive_MoveGyro_Degs(power_X, Target_D, degree, brake, async, _id);
      idx++;
      if (result == MMLower::Drive_RESULT::ERROR_Drive_Define) {
        if (idx >= Drive_retry_Period)
          return 0x07;
      } else if (result != MMLower::Drive_RESULT::OK) {
        if (idx >= Drive_retry_Period)
          return 0x01;
      } else {
        return 0x00;
      }
    }

    if (async == false) {
      bool stats[2];
      stats[0] = true;
      while (stats[0]) {
        delay(10);
        mmL.Get_Drive_isTaskDone(_id, stats);
      }
	  
	  #ifdef ENABLE_DRIVEDC_BRAKE_DELAY
      delay(DRIVEDC_BRAKE_DELAY_MS); //Give motor some time to stop.
      #endif
    }

    return 0x00;

  }

  /**
   * @brief Sets the power level with gyro guidance for specific time.
   * 
   * Robot moves with gyro sensor feedback to maintain heading for specific time duration
   * 
   * @param power The power level to set (-100 to 100).
   * @param Target Target Degree of the IMU Yaw (-360 ~ +360)
   * @param Time_S The Period(secs) of the motor running
   * @param brake True(Brake) / false(coast)
   * @param async True(non-block) / false(block) - default: false
   * @return 0x00 if the param was successfully set, error code otherwise.
   */
  uint8_t MoveGyroTime(int16_t power, int16_t Target, float Time_S, bool brake, bool async = false) {
    Time_S = Time_S * 1000; // to milliseconds
	MMLower::Drive_RESULT result = mmL.Set_Drive_MoveGyro_Time(power, Target, Time_S, brake, async, _id);

    uint8_t idx = 0;
    while (result != MMLower::Drive_RESULT::OK) {
      delay(2);
      result = mmL.Set_Drive_MoveGyro_Time(power, Target, Time_S, brake, async, _id);
      idx++;
      if (result == MMLower::Drive_RESULT::ERROR_Drive_Define) {
        if (idx >= Drive_retry_Period)
          return 0x07;
      } else if (result != MMLower::Drive_RESULT::OK) {
        if (idx >= Drive_retry_Period)
          return 0x01;
      } else {
        return 0x00;
      }
    }

    if (async == false) {
      bool stats[2];
      stats[0] = true;
      while (stats[0]) {
        delay(10);
        mmL.Get_Drive_isTaskDone(_id, stats);
      }
	  
	  #ifdef ENABLE_DRIVEDC_BRAKE_DELAY
      delay(DRIVEDC_BRAKE_DELAY_MS); //Give motor some time to stop.
      #endif
    }

    return 0x00;
  }

  /**
   * @brief Turns the robot to a specific gyro heading.
   * 
   * Robot turns in place using gyro sensor feedback to reach target heading
   * 
   * @param power The power level to set (-100 to 100).
   * @param Target_D Target Degree of the IMU Yaw (-360 ~ +360)
   * @param mode 0:single motor / 1:two motor
   * @param brake True(Brake) / false(coast)
   * @param async True(non-block) / false(block) - default: false
   * @return 0x00 if the param was successfully set, error code otherwise.
   */
  uint8_t TurnGyro(int16_t power, int16_t Target_D, uint8_t mode, bool brake, bool async = false) {
    MMLower::Drive_RESULT result = mmL.Set_Drive_TurnGyro(power, Target_D, mode, brake, async, _id);

    if (result != MMLower::Drive_RESULT::OK) {
      delay(1);
      result = mmL.Set_Drive_TurnGyro(power, Target_D, mode, brake, async, _id);
    }

    if (result == MMLower::Drive_RESULT::ERROR_Drive_Define) {
      return 0x07;
    } else if (result != MMLower::Drive_RESULT::OK) {
      return 0x01;
    }

    if (async == false) {
      bool stats[2];
      stats[0] = true;
      while (stats[0]) {
        delay(10);
        mmL.Get_Drive_isTaskDone(_id, stats);
      }
	  
	  #ifdef ENABLE_DRIVEDC_BRAKE_DELAY
      delay(DRIVEDC_BRAKE_DELAY_MS); //Give motor some time to stop.
      #endif
    }

    return 0x00;
  }

  /**
   * @brief Gets the estimated rotation counter of the drivebase. (not degrees)
   * 
   * @return The current encoder counter value.
   */
  int32_t getCounter(void) {
    int32_t counter = 0;
    MMLower::Drive_RESULT result = mmL.Get_Drive_EncoderCounter(_id, counter);

    if (result != MMLower::Drive_RESULT::OK) {
      delay(1);
      result = mmL.Get_Drive_EncoderCounter(_id, counter);
    }

    return counter;
  }

  /**
   * @brief Gets the estimated degrees of the drivebase.
   * 
   * @return The calculated degrees of rotation.
   */
  int32_t getDegrees(void) {
    int32_t degrees = 0;
    MMLower::Drive_RESULT result = mmL.Get_Drive_Degrees(_id, degrees);

    if (result != MMLower::Drive_RESULT::OK) {
      delay(1);
      result = mmL.Get_Drive_Degrees(_id, degrees);
    }

    return degrees;
  }

  /**
   * @brief Resets the drivebase encoder counter/degrees to zero.
   * 
   * @return 0x00 if the counter was successfully reset, error code otherwise.
   */
  uint8_t resetCounter(void) {
    MMLower::Drive_RESULT result = mmL.Set_Drive_Reset_Count(_id, true);

    if (result == MMLower::Drive_RESULT::ERROR_Drive_Define) {
      return 0x07;
    } else if (result == MMLower::Drive_RESULT::OK) {
      return 0x00;
    } else {
      return 0x01;
    }
  }

  /**
   * @brief Sets the brake mode for the DC motor.
   * 
   * Like LEGO EV3/SPIKE, True is Brake, False is Coast.
   * 
   * @param brakeType True to engage the brake, false to coast the motor.
   * @return 0x00 if the brake was successfully set, error code otherwise.
   */
  uint8_t brake(bool brakeType) {
    MMLower::Drive_RESULT result = mmL.Set_Drive_Brake(brakeType, _id);

    if (result == MMLower::Drive_RESULT::ERROR_Drive_Define) {
      return 0x07;
    } else if (result == MMLower::Drive_RESULT::OK) {
      return 0x00;
    } else {
      return 0x01;
    }
  }

  /**
   * @brief Checks if the previous task has completed.
   * 
   * @return True if the previous task is done, false if still running.
   */
  bool isPrevTaskDone(void) {
    bool stsu[2];

    MMLower::Drive_RESULT result = mmL.Get_Drive_isTaskDone(_id, stsu);
    return stsu[0] ? false : true;

  }

  private: uint8_t _id_left,
  _id_right;
  uint8_t _id;
  MMLower::DIR _dir_left,
  _dir_right;
};

#endif // MINIR4DRIVEDC_H