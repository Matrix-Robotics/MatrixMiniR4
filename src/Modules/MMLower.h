/**
 * @file MMLower.h
 * @brief Handling the Lower MCU (STM32) communication.
 * @author MATRIX Robotics
 */
#ifndef MMLOWER_H
#define MMLOWER_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#define MR4_DEBUG_ENABLE false
#define MR4_DEBUG_SERIAL Serial
#if MR4_DEBUG_ENABLE
#    define MR4_DEBUG_HEADER()     MR4_DEBUG_SERIAL.println(F("\nBMR4\n"))
#    define MR4_DEBUG_TAIL()       MR4_DEBUG_SERIAL.println(F("\nEMR4\n"))
#    define MR4_DEBUG_PRINT(...)   MR4_DEBUG_SERIAL.print(__VA_ARGS__)
#    define MR4_DEBUG_PRINTLN(...) MR4_DEBUG_SERIAL.println(__VA_ARGS__)
#    define MR4_DEBUG_PRINT_HEADER(...)            \
        do {                                       \
            MR4_DEBUG_HEADER();                    \
            MR4_DEBUG_SERIAL.println(__VA_ARGS__); \
        } while (0)
#    define MR4_DEBUG_PRINT_TAIL(...)              \
        do {                                       \
            MR4_DEBUG_SERIAL.println(__VA_ARGS__); \
            MR4_DEBUG_TAIL();                      \
        } while (0)
#else
#    define MR4_DEBUG_HEADER()
#    define MR4_DEBUG_TAIL()
#    define MR4_DEBUG_PRINT_HEADER(...)
#    define MR4_DEBUG_PRINT_TAIL(...)
#    define MR4_DEBUG_PRINT(...)
#    define MR4_DEBUG_PRINTLN(...)
#endif

#define MatrixR4_COMM_LEAD 0x7B

#define MatrixR4_SERVO_NUM    4
#define MatrixR4_DC_MOTOR_NUM 4
#define MatrixR4_ENCODER_NUM  4
#define MatrixR4_BUTTON_NUM   2

#define DIR_REVERSE (MatrixMiniR4::DIR::REVERSE)
#define DIR_FORWARD (MatrixMiniR4::DIR::FORWARD)

/**
 * @brief Handling the Lower MCU (STM32) communication.
 */
class MMLower
{
public:
    MMLower(uint8_t rx, uint8_t tx, uint32_t baudrate);

    enum class COMM_STATE
    {
        WAIT_LEAD,
        WAIT_NOT_LEAD,
        WAIT_CMD,
        ERROR,
    };

    enum class COMM_CMD
    {
        NONE = 0x00,
        // Setting-Init
        SET_DC_MOTOR_DIR = 0x01,
        SET_ENCODER_DIR,
        SET_SERVO_DIR,
        SET_DC_MOTOR_SPEED_RANGE,
        SET_SERVO_PULSE_RANGE,
        SET_SERVO_ANGLE_RANGE,
        SET_BUTTON_INIT,
        SET_ENCODER_ECHO_MODE,
        SET_IMU_ECHO_MODE,
        SET_IMU_INIT,
        SET_POWER_PARAM,
		SET_ENCODER_PPR_MAXSPEED,			//  2025/07/07
		SET_ALL_ENCODER_PPR,				//  2025/05/22
		SET_IMU_Calib_Data,					//  2025/05/22	
		
        // Setting-Commonly used
        SET_DC_MOTOR_POWER = 0x11,
        SET_DC_MOTOR_SPEED,
        SET_DC_MOTOR_ROTATE,
        SET_ALL_DC_MOTOR_SPEED,
        SET_SERVO_ANGLE,
        SET_ALL_SERVO_ANGLE,
        SET_MOVE_DISTANCE,
        SET_ENCODER_RESET_COUNTER,
        SET_STATE_LED,
        SET_IMU_TO_ZERO,
        SET_PID_PARAM,
        SET_DC_BRAKE,
		SET_ALL_DC_BRAKE,					//  2025/05/22
		SET_ALL_DC_MOTOR_POWER,				//  2025/05/22
		
        // Getting
        GET_BUTTON_STATE = 0x21,
        GET_BUTTONS_STATE,
        GET_ENCODER_COUNTER,
        GET_ALL_ENCODER_COUNTER,
        GET_IMU_EULER,
        GET_IMU_GYRO,
        GET_IMU_ACC,
        GET_POWER_INFO,
        GET_ROTATE_STATE,
		GET_SPEED_ALL_DC_MOTOR,				//  2025/05/22
		GET_IMU_ACC_NOcal,					//  2025/05/22
		GET_ENCODER_DEGREES,				//  2025/07/15
		
        // Auto-Send
        AUTO_SEND_BUTTON_STATE = 0x31,
        AUTO_SEND_ENCODER_COUNTER,
        AUTO_SEND_IMU_EULER,
        AUTO_SEND_IMU_GYRO,
        AUTO_SEND_IMU_ACC,
		
		// New Function 					//  2025/05/22
		SET_DC_BRAKE_TYPE			= 0x41,
		SET_DC_ALL_BRAKE_TYPE,
		SET_DC_TWO_MOTOR_PARAM,
		SET_DC_TWO_MoveSync_PID,
		SET_DC_TWO_MoveGyro_PID,
		SET_DC_TWO_TurnGyro_PID,
		SET_DC_TWO_MOTOR_Reset_count,
		SET_DC_TWO_MOTOR_PPR,
		SET_DC_MOTOR_TYPE,

		// New Function 					//  2025/05/22
		SET_Drive_Move				= 0x51,
		SET_Drive_MoveDegs,
		SET_Drive_MoveTime,

		SET_Drive_MoveSync,
		SET_Drive_MoveSyncDegs,
		SET_Drive_MoveSyncDegsACC,
		SET_Drive_MoveSyncTime,

		SET_Drive_Gyro,
		SET_Drive_GyroDegs,
		SET_Drive_GyroTime,

		SET_Drive_Turn,
		SET_Drive_Brake,
		GET_Task_Done_Status,
		GET_Drive_Degress,
		GET_Drive_Counter,
		

        // Other-Info
        ECHO_TEST        = 0xFF,
        F_VERSION        = 0xFE,
        F_BUILD_DAY      = 0xFD,
        F_DESCRIPTOR     = 0xFC,
        READ_MODEL_INDEX = 0xFB,
        READ_ALL_INFO    = 0xFA,
        RUN_AUTO_QC      = 0xF9,
    };

    enum class BTN_STATE
    {
        NOKEY,
        F_EDGE,
        REPEAT,
        PRESSED,
        R_EDGE,
    };

    enum class DIR
    {
        REVERSE,
        FORWARD,
    };

    enum class BUTTON_ECHO_MODE
    {
        PASSIVE,
        ACTIVE,
        MAX,
    };

    enum class ENCODER_ECHO_MODE
    {
        PASSIVE,
        ACTIVE,
        MAX,
    };

    enum class IMU_ECHO_MODE
    {
        PASSIVE,
        TIMING,
        ACTIVE,
        MAX,
    };

    enum class IMU_ACC_FSR
    {
        _2G,
        _4G,
        _8G,
        _16G,
    };

    enum class IMU_GYRO_FSR
    {
        _250DPS,
        _500DPS,
        _1000DPS,
        _2000DPS,
    };

    enum class IMU_ODR
    {
        _10_SPS,
        _20_SPS,
        _25_SPS,
        _50_SPS,
        _100_SPS,
        _125_SPS,
        _250_SPS,
        _500_SPS,
        _1000_SPS,
        _2000_SPS,
        _4000_SPS,
        _8000_SPS,
    };

    enum class IMU_FIFO
    {
        ENABLE,
        DISABLE,
    };

    enum class MOVE_TYPE
    {
        // 兩輪差速
        DIFF,
        // 四輪全向
        OMNI,
    };

    enum class MOVE_ACTION
    {
        STOP,
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
    };

    enum class RESULT
    {
        OK,
        ERROR,
        ERROR_SOFTSERIAL_BEGIN,
        ERROR_INIT,
        ERROR_WAIT_TIMEOUT,
        ERROR_READ_TIMEOUT,

        // SERVO PULSE RANGE
        ERROR_SERVO_MIN_PULSE,
        ERROR_SERVO_MAX_PULSE,

        // SERVO ANGLE RANGE
        ERROR_SERVO_MIN_ANGLE,
        ERROR_SERVO_MAX_ANGLE,

        ERROR_MODE,
        ERROR_INTERVAL,

        // IMU Init
        ERROR_IMU_ACC_FSR,
        ERROR_IMU_GYRO_FSR,
        ERROR_IMU_ODR,

        // Motor Speed
        ERROR_MOTOR_SPEED,
        ERROR_MOTOR_POWER,
        ERROR_MOTOR1_SPEED,
        ERROR_MOTOR2_SPEED,
        ERROR_MOTOR3_SPEED,
        ERROR_MOTOR4_SPEED,

        // Servo Angle
        ERROR_SERVO_ANGLE,
        ERROR_SERVO1_ANGLE,
        ERROR_SERVO2_ANGLE,
        ERROR_SERVO3_ANGLE,
        ERROR_SERVO4_ANGLE,

        // Move Distance
        ERROR_MOVE_ACTION,
        ERROR_MOVE_SPEED,
        ERROR_MOVE_ENCODER,

        // RunAutoQC
        ERROR_QC_IMU,

        ERROR_POWER_VOLT_RANGE,
    };
	
	enum class Drive_RESULT
    {
		OK						= 0x00,
        ERROR,
        ERROR_SOFTSERIAL_BEGIN,
        ERROR_INIT,
        ERROR_WAIT_TIMEOUT,
        ERROR_READ_TIMEOUT,
		ERROR_Drive_Define 		= 0x07,
		ERROR_Drive_Param,
		ERROR_Drive_IMU_idle,
		ERROR_MOTOR_SPEED		= 0x0F,
		ERROR_MOTOR_POWER		= 0x10,
		
	};

    typedef struct
    {
        DIR      m1_dir;
        DIR      m2_dir;
        DIR      m3_dir;
        DIR      m4_dir;
		
        uint16_t m1_speed;
        uint16_t m2_speed;
        uint16_t m3_speed;
        uint16_t m4_speed;
		
        uint16_t m1_power;
        uint16_t m2_power;
        uint16_t m3_power;
        uint16_t m4_power;				
		
    } Motors_Param_t;

    typedef struct
    {
        String  fwVersion;
        String  fwBuildDay;
        uint8_t modelIndex;
    } AllInfo_t;

    typedef void (*BtnChgCallback)(uint8_t num, BTN_STATE newState);

    RESULT Init(uint32_t timeout_ms = 1000);
    // Application API
    // Setting-Init
    RESULT SetDCMotorDir(uint8_t num, DIR dir);
    RESULT SetEncoderDir(uint8_t num, DIR dir);
    RESULT SetServoDir(uint8_t num, DIR dir);
    RESULT SetDCMotorSpeedRange(uint8_t num, uint16_t min, uint16_t max);
    RESULT SetServoPulseRange(uint8_t num, uint16_t min, uint16_t max);
    RESULT SetServoAngleRange(uint8_t num, uint16_t min, uint16_t max);
    RESULT SetIMUEchoMode(IMU_ECHO_MODE mode, uint16_t echoIntervalMs);
    RESULT SetIMUInit(IMU_ACC_FSR accFSR, IMU_GYRO_FSR gyroFSR, IMU_ODR odr, IMU_FIFO fifo);
    RESULT SetPowerParam(float fullVolt, float cutOffVolt, float alarmVolt);
    RESULT SetStateLED(uint8_t brightness, uint32_t colorRGB);
    RESULT SetIMUToZero(void);
	RESULT SetIMU_Calib_data(float * bufdata);						//  2025/05/22	
	RESULT SetEncode_PPR_MaxRPM(uint8_t num, uint16_t ppr, uint16_t Maxspeed);				//  2025/07/07
	RESULT SetALL_Encode_PPR(uint16_t * ppr);						//  2025/05/24	
    // Setting-Commonly used
    RESULT SetDCMotorPower(uint8_t num, int16_t power);
    RESULT SetDCMotorSpeed(uint8_t num, int16_t speed);
    RESULT SetDCMotorRotate(uint8_t num, int16_t maxSpeed, uint16_t degree);
    RESULT SetAllDCMotorSpeed(Motors_Param_t param);
    RESULT SetServoAngle(uint8_t num, uint16_t angle);
    RESULT SetAllServoAngle(uint16_t angle1, uint16_t angle2, uint16_t angle3, uint16_t angle4);
    RESULT SetMoveDistance(MOVE_TYPE type, MOVE_ACTION action, uint16_t speed, uint16_t enCounter);
    RESULT SetEncoderResetCounter(uint8_t num);
    RESULT SetPIDParam(uint8_t num, uint8_t pidNum, float kp, float ki, float kd);
    RESULT SetDCBrake(uint8_t num);
	RESULT SetALLDCBrake(void);										//  2025/05/22	
	RESULT SetDC_Type_Brake(uint8_t num, uint8_t type);				//  2025/05/23
	RESULT SetALLDC_Type_Brake(uint8_t * type);						//  2025/05/23
	RESULT SetAllDCMotorPower(Motors_Param_t param);				//  2025/05/23
    // Getting
    RESULT GetButtonState(uint8_t num, bool& btnState);
    RESULT GetButtonsState(bool* btnsState);
    RESULT GetEncoderCounter(uint8_t num, int32_t& enCounter);
    RESULT GetAllEncoderCounter(int32_t* enCounter);
    RESULT GetIMUEuler(double& roll, double& pitch, double& yaw);
    RESULT GetIMUGyro(double& x, double& y, double& z);
    RESULT GetIMUAcc(double& x, double& y, double& z);
    RESULT GetPowerInfo(float& curVolt, float& curVoltPerc);
    RESULT GetRotateState(uint8_t num, bool& isEnd);
	RESULT GetALLEncoderSpeed(int32_t * enSpeed);					//  2025/05/22	
	RESULT Get_IMU_nancalib_acc(float * accdata);					//  2025/05/22	
	RESULT GetEncoderDegrees(uint8_t num, int32_t& enDeges);		//  2025/07/15	
    // Other-Info
    RESULT EchoTest(void);
    RESULT GetFWVersion(String& version);
    RESULT GetFWBuildDay(String& date);
    RESULT GetFWDescriptor(String& descriptor);
    RESULT GetModelIndex(uint8_t& index);
    RESULT GetAllInfo(AllInfo_t& info);
    RESULT RunAutoQC(void);
	
	// Drive DC Function											// 2025/05/30
	Drive_RESULT Set_Drive2Motor_PARAM(uint8_t m1_num, uint8_t m2_num, DIR m1_dir, DIR m2_dir, uint8_t num);
	RESULT Set_Drive_MoveSync_PID(float Kp, float Ki, float Kd, uint8_t num);
	RESULT Set_Drive_MoveGyro_PID(float Kp, float Ki, float Kd, uint8_t num);
	RESULT Set_Drive_MoveTurn_PID(float Kp, float Ki, float Kd, uint8_t num);
	Drive_RESULT Set_Drive_Reset_Count(uint8_t num, bool reset);
	Drive_RESULT Set_Drive_Encode_PPR(uint8_t num, uint16_t* ppr);	
	Drive_RESULT Set_Drive_Motor_Type(uint8_t num, uint8_t type);
	
	Drive_RESULT Set_Drive_Move_Func(int16_t power_left, int16_t power_right, uint8_t num);
	Drive_RESULT Set_Drive_Move_Degs(int16_t power_left, int16_t power_right, uint16_t Degree_c, bool brake, bool async, uint8_t num);
	Drive_RESULT Set_Drive_Move_Time(int16_t power_left, int16_t power_right, uint32_t Time_mS, bool brake, bool async, uint8_t num);
	
	Drive_RESULT Set_Drive_MoveSync_Func(int16_t power_left, int16_t power_right, uint8_t num);
	Drive_RESULT Set_Drive_MoveSync_Degs(int16_t power_left, int16_t power_right, uint16_t Degree_c, bool brake, bool async, uint8_t num);
	Drive_RESULT Set_Drive_MoveSync_Time(int16_t power_left, int16_t power_right, uint32_t Time_mS, bool brake, bool async, uint8_t num);

	Drive_RESULT Set_Drive_MoveGyro_Func(int16_t power, int16_t Target_dri, uint8_t num);
	Drive_RESULT Set_Drive_MoveGyro_Degs(int16_t power, int16_t Target_dri, uint16_t Degree_c, bool brake, bool async, uint8_t num);
	Drive_RESULT Set_Drive_MoveGyro_Time(int16_t power, int16_t Target_dri, uint32_t Time_mS, bool brake, bool async, uint8_t num);
	
	Drive_RESULT Set_Drive_TurnGyro(int16_t power, int16_t Target_dri, uint8_t mode, bool brake, bool async, uint8_t num);
	
	Drive_RESULT Set_Drive_Brake(bool brake, uint8_t num);	
	Drive_RESULT Get_Drive_isTaskDone(uint8_t num, bool * isEnd);
	Drive_RESULT Get_Drive_EncoderCounter(uint8_t num, int32_t& enCounter);
	Drive_RESULT Get_Drive_Degrees(uint8_t num, int32_t& Degs);

	

    void loop(void);
    void onBtnChg(BtnChgCallback callback);

    // TODO: 外部存取?
    // Encoders
    int32_t enCounter[MatrixR4_ENCODER_NUM];
    // IMU
    double imuGyroX, imuGyroY, imuGyroZ;
    double imuAccX, imuAccY, imuAccZ;

private:
    uint32_t        _baudrate;
    SoftwareSerial* commSerial;
    BtnChgCallback  callbackFunc;

    void CommSendData(COMM_CMD cmd, uint8_t* data = NULL, uint16_t size = 0);
    void CommSendData(COMM_CMD cmd, uint8_t data);
    bool CommReadData(uint8_t* data, uint16_t size = 1, uint32_t timeout_ms = 10);
    bool WaitData(COMM_CMD cmd = COMM_CMD::NONE, uint32_t timeout_ms = 0);
    void HandleCommand(uint8_t cmd);
};

extern MMLower mmL;

#endif   // MMLOWER_H
