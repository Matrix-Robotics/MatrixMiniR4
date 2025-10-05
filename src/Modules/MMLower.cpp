/**
 * @file MMLower.cpp
 * @brief Handling the Lower MCU (STM32) communication.
 * @author MATRIX Robotics
 */
#include "MMLower.h"
#include "Util/BitConverter.h"

MMLower::MMLower(uint8_t rx, uint8_t tx, uint32_t baudrate)
    : _baudrate(baudrate)
{
    commSerial = new SoftwareSerial(rx, tx);
}

MMLower::RESULT MMLower::Init(uint32_t timeout_ms)
{
    MR4_DEBUG_PRINT_HEADER(F("[Init]"));

	commSerial->begin(_baudrate, SERIAL_8N1);
	
	delay(1000);
	
	uint32_t asd = 0;

    //asd = millis() + 5000;
    while (asd < 50) {
		
        RESULT result = EchoTest();
		delay(250);
        if (result == RESULT::OK) {
            MR4_DEBUG_PRINT_TAIL(F("OK"));
            return RESULT::OK;
        } else {
            MR4_DEBUG_PRINT(F("EchoTest Failed! Result: "));
            MR4_DEBUG_PRINTLN((int)result);
        }
		asd++;
    }
    
	MR4_DEBUG_PRINT_TAIL(F("ERROR_INIT"));
    return RESULT::ERROR_INIT;
	//return RESULT::OK;
}

MMLower::RESULT MMLower::SetDCMotorDir(uint8_t num, DIR dir)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetDCMotorDir]"));

    uint8_t data[2] = {(1 << --num), (uint8_t)dir};
    CommSendData(COMM_CMD::SET_DC_MOTOR_DIR, data, 2);
    if (!WaitData(COMM_CMD::SET_DC_MOTOR_DIR, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetEncoderDir(uint8_t num, DIR dir)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetEncoderDir]"));

    uint8_t data[2] = {(1 << --num), (uint8_t)dir};
    CommSendData(COMM_CMD::SET_ENCODER_DIR, data, 2);
    if (!WaitData(COMM_CMD::SET_ENCODER_DIR, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetServoDir(uint8_t num, DIR dir)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetServoDir]"));

    uint8_t data[2] = {(1 << --num), (uint8_t)dir};
    CommSendData(COMM_CMD::SET_SERVO_DIR, data, 2);
    if (!WaitData(COMM_CMD::SET_SERVO_DIR, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetDCMotorSpeedRange(uint8_t num, uint16_t min, uint16_t max)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetDCMotorSpeedRange]"));

    uint8_t data[5];
    data[0] = (1 << --num);
    BitConverter::GetBytes(data + 1, min);
    BitConverter::GetBytes(data + 3, max);
    CommSendData(COMM_CMD::SET_DC_MOTOR_SPEED_RANGE, data, 5);
    if (!WaitData(COMM_CMD::SET_DC_MOTOR_SPEED_RANGE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetServoPulseRange(uint8_t num, uint16_t min, uint16_t max)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetServoPulseRange]"));

    uint8_t data[5];
    data[0] = (1 << --num);
    BitConverter::GetBytes(data + 1, min);
    BitConverter::GetBytes(data + 3, max);
    CommSendData(COMM_CMD::SET_SERVO_PULSE_RANGE, data, 5);
    if (!WaitData(COMM_CMD::SET_SERVO_PULSE_RANGE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_SERVO_MIN_PULSE"));
        return RESULT::ERROR_SERVO_MIN_PULSE;
    }
    if (b[0] == 0x03) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_SERVO_MAX_PULSE"));
        return RESULT::ERROR_SERVO_MAX_PULSE;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetServoAngleRange(uint8_t num, uint16_t min, uint16_t max)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetServoAngleRange]"));

    uint8_t data[5];
    data[0] = (1 << --num);
    BitConverter::GetBytes(data + 1, min);
    BitConverter::GetBytes(data + 3, max);
    CommSendData(COMM_CMD::SET_SERVO_ANGLE_RANGE, data, 5);
    if (!WaitData(COMM_CMD::SET_SERVO_ANGLE_RANGE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_SERVO_MIN_ANGLE"));
        return RESULT::ERROR_SERVO_MIN_ANGLE;
    }
    if (b[0] == 0x03) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_SERVO_MAX_ANGLE"));
        return RESULT::ERROR_SERVO_MAX_ANGLE;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetIMUEchoMode(IMU_ECHO_MODE mode, uint16_t echoIntervalMs)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetIMUEchoMode]"));

    uint8_t data[3];
    data[0] = (uint8_t)mode;
    BitConverter::GetBytes(data + 1, echoIntervalMs);
    CommSendData(COMM_CMD::SET_IMU_ECHO_MODE, data, 3);
    if (!WaitData(COMM_CMD::SET_IMU_ECHO_MODE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MODE"));
        return RESULT::ERROR_MODE;
    }
    if (b[0] == 0x03) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_INTERVAL"));
        return RESULT::ERROR_INTERVAL;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetIMUInit(
    IMU_ACC_FSR accFSR, IMU_GYRO_FSR gyroFSR, IMU_ODR odr, IMU_FIFO fifo)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetIMUInit]"));

    uint8_t data[4];
    data[0] = (uint8_t)accFSR;
    data[1] = (uint8_t)gyroFSR;
    data[2] = (uint8_t)odr;
    data[3] = (uint8_t)fifo;
    CommSendData(COMM_CMD::SET_IMU_INIT, data, 4);
    if (!WaitData(COMM_CMD::SET_IMU_INIT, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_IMU_ACC_FSR"));
        return RESULT::ERROR_IMU_ACC_FSR;
    }
    if (b[0] == 0x03) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_IMU_GYRO_FSR"));
        return RESULT::ERROR_IMU_GYRO_FSR;
    }
    if (b[0] == 0x04) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_IMU_ODR"));
        return RESULT::ERROR_IMU_ODR;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetPowerParam(float fullVolt, float cutOffVolt, float alarmVolt)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetPowerParam]"));

    uint8_t data[3];
    data[0] = (uint8_t)(fullVolt * 10.0f);
    data[1] = (uint8_t)(cutOffVolt * 10.0f);
    data[2] = (uint8_t)(alarmVolt * 10.0f);
    CommSendData(COMM_CMD::SET_POWER_PARAM, data, 3);
    if (!WaitData(COMM_CMD::SET_POWER_PARAM, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1, 5)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_POWER_VOLT_RANGE"));
        return RESULT::ERROR_POWER_VOLT_RANGE;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}
// Setting-Commonly used

MMLower::RESULT MMLower::SetDCMotorPower(uint8_t num, int16_t power)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetDCMotorPower]"));

    uint8_t data[4];
    data[0] = (1 << --num);
    data[1] = 0;   // Ma
    BitConverter::GetBytes(data + 2, power);
    CommSendData(COMM_CMD::SET_DC_MOTOR_POWER, data, 4);
    if (!WaitData(COMM_CMD::SET_DC_MOTOR_POWER, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR_POWER"));
        return RESULT::ERROR_MOTOR_POWER;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetDCMotorSpeed(uint8_t num, int16_t speed)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetDCMotorSpeed]"));

    uint8_t data[4];
    data[0] = (1 << --num);
    data[1] = 0;   // Ma
    BitConverter::GetBytes(data + 2, speed);
    CommSendData(COMM_CMD::SET_DC_MOTOR_SPEED, data, 4);
    if (!WaitData(COMM_CMD::SET_DC_MOTOR_SPEED, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR_SPEED"));
        return RESULT::ERROR_MOTOR_SPEED;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetDCMotorRotate(uint8_t num, int16_t maxSpeed, uint16_t degree)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetDCMotorRotate]"));

    uint8_t data[5];
    data[0] = (1 << --num);
    BitConverter::GetBytes(data + 1, maxSpeed);
    BitConverter::GetBytes(data + 3, degree);
    CommSendData(COMM_CMD::SET_DC_MOTOR_ROTATE, data, 5);
    if (!WaitData(COMM_CMD::SET_DC_MOTOR_ROTATE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetAllDCMotorSpeed(Motors_Param_t param)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetAllDCMotorSpeed]"));

    uint8_t data[12];
    data[0] = (uint8_t)param.m1_dir;
    data[0] |= ((uint8_t)param.m2_dir) << 1;
    data[0] |= ((uint8_t)param.m3_dir) << 2;
    data[0] |= ((uint8_t)param.m4_dir) << 3;
    BitConverter::GetBytes(data + 1, param.m1_speed);
    BitConverter::GetBytes(data + 3, param.m2_speed);
    BitConverter::GetBytes(data + 5, param.m3_speed);
    BitConverter::GetBytes(data + 7, param.m4_speed);
	
    CommSendData(COMM_CMD::SET_ALL_DC_MOTOR_SPEED, data, 10);
    if (!WaitData(COMM_CMD::SET_ALL_DC_MOTOR_SPEED, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR1_SPEED"));
        return RESULT::ERROR_MOTOR1_SPEED;
    }
    if (b[0] == 0x03) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR2_SPEED"));
        return RESULT::ERROR_MOTOR2_SPEED;
    }
    if (b[0] == 0x04) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR3_SPEED"));
        return RESULT::ERROR_MOTOR3_SPEED;
    }
    if (b[0] == 0x05) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR4_SPEED"));
        return RESULT::ERROR_MOTOR4_SPEED;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}
MMLower::RESULT MMLower::SetAllDCMotorPower(Motors_Param_t param)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetAllDCMotorPower]"));

    uint8_t data[12];
    data[0] = (uint8_t)param.m1_dir;
    data[0] |= ((uint8_t)param.m2_dir) << 1;
    data[0] |= ((uint8_t)param.m3_dir) << 2;
    data[0] |= ((uint8_t)param.m4_dir) << 3;
    BitConverter::GetBytes(data + 1, param.m1_power);
    BitConverter::GetBytes(data + 3, param.m2_power);
    BitConverter::GetBytes(data + 5, param.m3_power);
    BitConverter::GetBytes(data + 7, param.m4_power);
	
    CommSendData(COMM_CMD::SET_ALL_DC_MOTOR_POWER, data, 10);
    if (!WaitData(COMM_CMD::SET_ALL_DC_MOTOR_POWER, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR1_SPEED"));
        return RESULT::ERROR_MOTOR1_SPEED;
    }
    if (b[0] == 0x03) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR2_SPEED"));
        return RESULT::ERROR_MOTOR2_SPEED;
    }
    if (b[0] == 0x04) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR3_SPEED"));
        return RESULT::ERROR_MOTOR3_SPEED;
    }
    if (b[0] == 0x05) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR4_SPEED"));
        return RESULT::ERROR_MOTOR4_SPEED;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetServoAngle(uint8_t num, uint16_t angle)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetServoAngle]"));

    uint8_t data[3];
    data[0] = (1 << --num);
    BitConverter::GetBytes(data + 1, angle);
    CommSendData(COMM_CMD::SET_SERVO_ANGLE, data, 3);
    if (!WaitData(COMM_CMD::SET_SERVO_ANGLE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_SERVO_ANGLE"));
        return RESULT::ERROR_SERVO_ANGLE;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetAllServoAngle(
    uint16_t angle1, uint16_t angle2, uint16_t angle3, uint16_t angle4)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetAllServoAngle]"));

    uint8_t data[8];
    BitConverter::GetBytes(data + 0, angle1);
    BitConverter::GetBytes(data + 2, angle2);
    BitConverter::GetBytes(data + 4, angle3);
    BitConverter::GetBytes(data + 6, angle4);
    CommSendData(COMM_CMD::SET_ALL_SERVO_ANGLE, data, 8);
    if (!WaitData(COMM_CMD::SET_ALL_SERVO_ANGLE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_SERVO1_ANGLE"));
        return RESULT::ERROR_SERVO1_ANGLE;
    }
    if (b[0] == 0x03) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_SERVO2_ANGLE"));
        return RESULT::ERROR_SERVO2_ANGLE;
    }
    if (b[0] == 0x04) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_SERVO3_ANGLE"));
        return RESULT::ERROR_SERVO3_ANGLE;
    }
    if (b[0] == 0x05) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_SERVO4_ANGLE"));
        return RESULT::ERROR_SERVO4_ANGLE;
    }

    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetMoveDistance(
    MOVE_TYPE type, MOVE_ACTION action, uint16_t speed, uint16_t enCounter)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetMoveDistance]"));

    uint8_t data[6];
    data[0] = (uint8_t)type;
    data[1] = (uint8_t)action;
    BitConverter::GetBytes(data + 2, speed);
    BitConverter::GetBytes(data + 4, enCounter);
    CommSendData(COMM_CMD::SET_MOVE_DISTANCE, data, 6);
    if (!WaitData(COMM_CMD::SET_MOVE_DISTANCE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOVE_ACTION"));
        return RESULT::ERROR_MOVE_ACTION;
    }
    if (b[0] == 0x03) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOVE_SPEED"));
        return RESULT::ERROR_MOVE_SPEED;
    }
    if (b[0] == 0x04) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOVE_ENCODER"));
        return RESULT::ERROR_MOVE_ENCODER;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetEncoderResetCounter(uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetEncoderResetCounter]"));

    uint8_t data[1] = {(1 << --num)};
    CommSendData(COMM_CMD::SET_ENCODER_RESET_COUNTER, data, 1);
    if (!WaitData(COMM_CMD::SET_ENCODER_RESET_COUNTER, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetIMU_Calib_data(float * bufdata)
{
    uint8_t data[30];
	data[0] = 1;
	data[1] = 1;
    BitConverter::FloatGetBytes(data+2, bufdata[0]);
    BitConverter::FloatGetBytes(data+6, bufdata[1]);	
	BitConverter::FloatGetBytes(data+10, bufdata[2]);	
	BitConverter::FloatGetBytes(data+14, bufdata[3]);	
	BitConverter::FloatGetBytes(data+18, bufdata[4]);	
	BitConverter::FloatGetBytes(data+22, bufdata[5]);

    CommSendData(COMM_CMD::SET_IMU_Calib_Data, data, 26);
    if (!WaitData(COMM_CMD::SET_IMU_Calib_Data, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetPIDParam(uint8_t num, uint8_t pidNum, float kp, float ki, float kd)
{
    uint8_t data[8] = {(1 << --num), pidNum};
    BitConverter::GetBytes(data + 2, (uint16_t)(kp * 100.0f));
    BitConverter::GetBytes(data + 4, (uint16_t)(ki * 100.0f));
    BitConverter::GetBytes(data + 6, (uint16_t)(kd * 100.0f));

    CommSendData(COMM_CMD::SET_PID_PARAM, data, 8);
    if (!WaitData(COMM_CMD::SET_PID_PARAM, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetDCBrake(uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetDCBrake]"));

    uint8_t data[1] = {(1 << --num)};
    CommSendData(COMM_CMD::SET_DC_BRAKE, data, 1);
    if (!WaitData(COMM_CMD::SET_DC_BRAKE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetALLDCBrake(void)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetALLDCBrake]"));

    uint8_t data[2];
	data[0] = 1;
    CommSendData(COMM_CMD::SET_ALL_DC_BRAKE, data, 1);
    if (!WaitData(COMM_CMD::SET_ALL_DC_BRAKE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetALLDC_Type_Brake(uint8_t * type)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetALLDC_TypeBrake]"));

    uint8_t data[8];
	data[0] = 1;
	data[1] = type[0];
	data[2] = type[1];
	data[3] = type[2];
	data[4] = type[3];
    CommSendData(COMM_CMD::SET_DC_ALL_BRAKE_TYPE, data, 5);
    if (!WaitData(COMM_CMD::SET_DC_ALL_BRAKE_TYPE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetDC_Type_Brake(uint8_t num, uint8_t type)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetDC_TypeBrake]"));

    uint8_t data[3];
	data[0] = 1;
	data[1] = type;
    CommSendData(COMM_CMD::SET_DC_BRAKE_TYPE, data, 2);
    if (!WaitData(COMM_CMD::SET_DC_BRAKE_TYPE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetEncode_PPR_MaxRPM(uint8_t num, uint16_t ppr, uint16_t Maxspeed)
{
    MR4_DEBUG_PRINT_HEADER(F("[Set_Encode_PPR]"));

    uint8_t data[6];
	data[0] = num - 1;
	data[1] = 0;
	BitConverter::GetBytes(data + 2, (uint16_t)(ppr));
	BitConverter::GetBytes(data + 4, (uint16_t)(Maxspeed));
	
    CommSendData(COMM_CMD::SET_ENCODER_PPR_MAXSPEED, data, 6);
    if (!WaitData(COMM_CMD::SET_ENCODER_PPR_MAXSPEED, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetALL_Encode_PPR(uint16_t * ppr)
{
	MR4_DEBUG_PRINT_HEADER(F("[Set_ALL_Encode_PPR]"));
	
    uint8_t data[12];
	data[0] = 1;
	data[1] = 0;
	BitConverter::GetBytes(data + 2, (uint16_t)(ppr[0]));	
	BitConverter::GetBytes(data + 4, (uint16_t)(ppr[1]));	
	BitConverter::GetBytes(data + 6, (uint16_t)(ppr[2]));	
	BitConverter::GetBytes(data + 8, (uint16_t)(ppr[3]));	
	
    CommSendData(COMM_CMD::SET_ALL_ENCODER_PPR, data, 10);
    if (!WaitData(COMM_CMD::SET_ALL_ENCODER_PPR, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;	
}

MMLower::RESULT MMLower::SetStateLED(uint8_t brightness, uint32_t colorRGB)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetStateLED]"));

    uint8_t data[4];
    data[0] = brightness;
    data[1] = (uint8_t)(colorRGB >> 16);
    data[2] = (uint8_t)(colorRGB >> 8);
    data[3] = (uint8_t)(colorRGB);
    CommSendData(COMM_CMD::SET_STATE_LED, data, 4);
    if (!WaitData(COMM_CMD::SET_STATE_LED, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1, 10)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::SetIMUToZero(void)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetIMUToZero]"));

    CommSendData(COMM_CMD::SET_IMU_TO_ZERO);
    if (!WaitData(COMM_CMD::SET_IMU_TO_ZERO, 1000)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1, 10)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}
// Getting
MMLower::RESULT MMLower::GetButtonState(uint8_t num, bool& btnState)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetButtonState]"));

    uint8_t data[1] = {--num};
    CommSendData(COMM_CMD::GET_BUTTON_STATE, data, 1);
    if (!WaitData(COMM_CMD::GET_BUTTON_STATE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
	
	btnState = (bool)b[0];

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetButtonsState(bool* btnsState)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetButtonsState]"));

    CommSendData(COMM_CMD::GET_BUTTONS_STATE);
    if (!WaitData(COMM_CMD::GET_BUTTONS_STATE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[2];
    if (!CommReadData(b, 2)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    uint16_t flag = BitConverter::ToUInt16(b, 0);
    btnsState[0]  = (bool)(flag);
    btnsState[1]  = (bool)(flag >> 1);

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetALLEncoderSpeed(int32_t * enSpeed)
{
    MR4_DEBUG_PRINT_HEADER(F("[Get_ALL_Encoder_SPEED]"));

    CommSendData(COMM_CMD::GET_SPEED_ALL_DC_MOTOR);
    if (!WaitData(COMM_CMD::GET_SPEED_ALL_DC_MOTOR, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[17];
    if (!CommReadData(b, 17)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    enSpeed[0] = BitConverter::ToInt32(b, 1);
    enSpeed[1] = BitConverter::ToInt32(b, 5);
    enSpeed[2] = BitConverter::ToInt32(b, 9);
    enSpeed[3] = BitConverter::ToInt32(b, 13);

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}


MMLower::RESULT MMLower::Get_IMU_nancalib_acc(float * accdata)
{
    MR4_DEBUG_PRINT_HEADER(F("[Get_IMU_nancalib_acc]"));

    CommSendData(COMM_CMD::GET_IMU_ACC_NOcal);
    if (!WaitData(COMM_CMD::GET_IMU_ACC_NOcal, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[13];
    if (!CommReadData(b, 13)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    accdata[0] = BitConverter::Tofloat(b, 1);
    accdata[1] = BitConverter::Tofloat(b, 5);
    accdata[2] = BitConverter::Tofloat(b, 9);

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetEncoderDegrees(uint8_t num, int32_t& enDeges)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetEncoderDegrees]"));

    uint8_t data[1] = {--num};
    CommSendData(COMM_CMD::GET_ENCODER_DEGREES, data, 1);
    if (!WaitData(COMM_CMD::GET_ENCODER_DEGREES, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[4];
    if (!CommReadData(b, 4)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    enDeges = BitConverter::ToInt32(b, 0);

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetEncoderCounter(uint8_t num, int32_t& enCounter)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetEncoderCounter]"));

    uint8_t data[1] = {--num};
    CommSendData(COMM_CMD::GET_ENCODER_COUNTER, data, 1);
    if (!WaitData(COMM_CMD::GET_ENCODER_COUNTER, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[4];
    if (!CommReadData(b, 4)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    enCounter = BitConverter::ToInt32(b, 0);

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetAllEncoderCounter(int32_t* enCounter)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetAllEncoderCounter]"));

    CommSendData(COMM_CMD::GET_ALL_ENCODER_COUNTER);
    if (!WaitData(COMM_CMD::GET_ALL_ENCODER_COUNTER, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[16];
    if (!CommReadData(b, 16)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    enCounter[0] = BitConverter::ToInt32(b, 0);
    enCounter[1] = BitConverter::ToInt32(b, 4);
    enCounter[2] = BitConverter::ToInt32(b, 8);
    enCounter[3] = BitConverter::ToInt32(b, 12);

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetIMUEuler(double& roll, double& pitch, double& yaw)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetIMUEuler]"));

    CommSendData(COMM_CMD::GET_IMU_EULER);
    if (!WaitData(COMM_CMD::GET_IMU_EULER, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[6];
    if (!CommReadData(b, 6)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    roll  = BitConverter::ToInt16(b, 0) / 100.0f;
    pitch = BitConverter::ToInt16(b, 2) / 100.0f;
    yaw   = BitConverter::ToInt16(b, 4) / 100.0f;

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetIMUGyro(double& x, double& y, double& z)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetIMUGyro]"));

    CommSendData(COMM_CMD::GET_IMU_GYRO);
    if (!WaitData(COMM_CMD::GET_IMU_GYRO, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[6];
    if (!CommReadData(b, 6)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    x = BitConverter::ToInt16(b, 0) / 100.0f;
    y = BitConverter::ToInt16(b, 2) / 100.0f;
    z = BitConverter::ToInt16(b, 4) / 100.0f;

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetIMUAcc(double& x, double& y, double& z)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetIMUAcc]"));

    CommSendData(COMM_CMD::GET_IMU_ACC);
    if (!WaitData(COMM_CMD::GET_IMU_ACC, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[6];
    if (!CommReadData(b, 6)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    x = BitConverter::ToInt16(b, 0) / 1000.0f;
    y = BitConverter::ToInt16(b, 2) / 1000.0f;
    z = BitConverter::ToInt16(b, 4) / 1000.0f;

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetPowerInfo(float& curVolt, float& curVoltPerc)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetPowerInfo]"));

    CommSendData(COMM_CMD::GET_POWER_INFO);
    if (!WaitData(COMM_CMD::GET_POWER_INFO, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[3];
    if (!CommReadData(b, 3)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    uint16_t voltRaw = BitConverter::ToUInt16(b, 0);
    curVolt          = (float)voltRaw / 1000.0f;
    curVoltPerc      = (float)b[2];

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetRotateState(uint8_t num, bool& isEnd)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetRotateState]"));

    uint8_t data[1] = {--num};
    CommSendData(COMM_CMD::GET_ROTATE_STATE, data, 1);
    if (!WaitData(COMM_CMD::GET_ROTATE_STATE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    isEnd = b[0];

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}
// Other-Info
MMLower::RESULT MMLower::EchoTest(void)
{

    MR4_DEBUG_PRINT_HEADER(F("[EchoTest]"));

    uint8_t data[1] = {0x55};
    CommSendData(COMM_CMD::ECHO_TEST, data, 1);
    if (!WaitData(COMM_CMD::ECHO_TEST, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[8];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
	
    if (b[0] == data[0]) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }
    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::GetFWVersion(String& version)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetFWVersion]"));

    CommSendData(COMM_CMD::F_VERSION);
    if (!WaitData(COMM_CMD::F_VERSION, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    version = String(b[0] / 10.0f);

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetFWBuildDay(String& date)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetFWBuildDay]"));

    CommSendData(COMM_CMD::F_BUILD_DAY);
    if (!WaitData(COMM_CMD::F_BUILD_DAY, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[4];
    if (!CommReadData(b, 4)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    uint16_t year  = BitConverter::ToUInt16(b, 0);
    uint8_t  month = b[2];
    uint8_t  day   = b[3];

    char str[10];
    sprintf(str, "%04d-%02d-%02d", year, month, day);
    date = String(str);

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetFWDescriptor(String& descriptor)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetFWDescriptor]"));

    CommSendData(COMM_CMD::F_DESCRIPTOR);
    if (!WaitData(COMM_CMD::F_DESCRIPTOR, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    uint8_t len = b[0];
    uint8_t str[len + 1];
    if (!CommReadData(str, len)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    str[len]   = '\0';
    descriptor = String((char*)str);
    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetModelIndex(uint8_t& index)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetModelIndex]"));

    CommSendData(COMM_CMD::READ_MODEL_INDEX);
    if (!WaitData(COMM_CMD::READ_MODEL_INDEX, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    index = b[0];

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::GetAllInfo(AllInfo_t& info)
{
    MR4_DEBUG_PRINT_HEADER(F("[GetAllInfo]"));

    CommSendData(COMM_CMD::F_VERSION);
    if (!WaitData(COMM_CMD::F_VERSION, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[6];
    if (!CommReadData(b, 6)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    uint16_t year  = BitConverter::ToUInt16(b, 0);
    uint8_t  month = b[2];
    uint8_t  day   = b[3];

    char str[10];
    sprintf(str, "%04d-%02d-%02d", year, month, day);

    info.fwVersion  = String(b[0] / 10.0f);
    info.fwBuildDay = String(str);
    info.modelIndex = b[5];

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

MMLower::RESULT MMLower::RunAutoQC(void)
{
    MR4_DEBUG_PRINT_HEADER(F("[RunAutoQC]"));

    CommSendData(COMM_CMD::RUN_AUTO_QC);
    if (!WaitData(COMM_CMD::RUN_AUTO_QC, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }

    if ((b[0] & 0x01) == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_QC_IMU"));
        return RESULT::ERROR_QC_IMU;
    }

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return RESULT::OK;
}

//--------------------------------------------------------------//
//--------------------------------------------------------------//
//  New Drive DC 2 Motor Function  //
//--------------------------------------------------------------//
//--------------------------------------------------------------//
MMLower::Drive_RESULT MMLower::Set_Drive2Motor_PARAM(uint8_t m1_num, uint8_t m2_num, DIR m1_dir, DIR m2_dir, uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[SetDriveDC_2motor_Param]"));

    uint8_t data[5] = {m1_num, m2_num, (uint8_t)m1_dir, (uint8_t)m2_dir, (num - 1)};
	
    CommSendData(COMM_CMD::SET_DC_TWO_MOTOR_PARAM, data, 5);
    if (!WaitData(COMM_CMD::SET_DC_TWO_MOTOR_PARAM, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if(b[0] == 0x08){
        MR4_DEBUG_PRINT_TAIL(F("Drive_Param_ERROR"));
        return Drive_RESULT::ERROR_Drive_Param;		
    }else if(b[0] == 0x09){
        MR4_DEBUG_PRINT_TAIL(F("Drive_IMU_Idle"));
        return Drive_RESULT::ERROR_Drive_IMU_idle;		
	}
	
    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;

}

MMLower::RESULT MMLower::Set_Drive_MoveSync_PID(float Kp, float Ki, float Kd, uint8_t num)
{
    uint8_t data[13];
	BitConverter::FloatGetBytes(data, Kp);
	BitConverter::FloatGetBytes(data+4, Ki);
	BitConverter::FloatGetBytes(data+8, Kd);
	data[12] = num - 1;

    CommSendData(COMM_CMD::SET_DC_TWO_MoveSync_PID, data, 13);
    if (!WaitData(COMM_CMD::SET_DC_TWO_MoveSync_PID, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    return RESULT::ERROR;
}

MMLower::RESULT MMLower::Set_Drive_MoveGyro_PID(float Kp, float Ki, float Kd, uint8_t num)
{
    uint8_t data[13];
	BitConverter::FloatGetBytes(data, Kp);
	BitConverter::FloatGetBytes(data+4, Ki);
	BitConverter::FloatGetBytes(data+8, Kd);
	data[12] = num - 1;

    CommSendData(COMM_CMD::SET_DC_TWO_MoveGyro_PID, data, 13);
    if (!WaitData(COMM_CMD::SET_DC_TWO_MoveGyro_PID, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    return RESULT::ERROR;
    return RESULT::ERROR;
}

MMLower::RESULT MMLower::Set_Drive_MoveTurn_PID(float Kp, float Ki, float Kd, uint8_t num)
{
    uint8_t data[13];
	BitConverter::FloatGetBytes(data, Kp);
	BitConverter::FloatGetBytes(data+4, Ki);
	BitConverter::FloatGetBytes(data+8, Kd);
	data[12] = num - 1;

    CommSendData(COMM_CMD::SET_DC_TWO_TurnGyro_PID, data, 13);
    if (!WaitData(COMM_CMD::SET_DC_TWO_TurnGyro_PID, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return RESULT::ERROR_READ_TIMEOUT;
    }
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return RESULT::OK;
    }

    return RESULT::ERROR;
}

MMLower::Drive_RESULT MMLower::Set_Drive_Encode_PPR(uint8_t num, uint16_t* ppr)
{
	MR4_DEBUG_PRINT_HEADER(F("[Set_Drive_Encode_PPR]"));
	
    uint8_t data[12];

	BitConverter::GetBytes(data, (uint16_t)(ppr[0]));	
	BitConverter::GetBytes(data + 2, (uint16_t)(ppr[1]));	
	data[4] = num - 1;
	
    CommSendData(COMM_CMD::SET_DC_TWO_MOTOR_PPR, data, 5);
    if (!WaitData(COMM_CMD::SET_DC_TWO_MOTOR_PPR, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }
	
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;	
}

MMLower::Drive_RESULT MMLower::Set_Drive_Motor_Type(uint8_t num, uint8_t type)
{
	MR4_DEBUG_PRINT_HEADER(F("[Set_Drive_Motor_Type]"));
	
    uint8_t data[2];

	data[0] = num - 1;
	data[1] = type;
	
    CommSendData(COMM_CMD::SET_DC_MOTOR_TYPE, data, 2);
    if (!WaitData(COMM_CMD::SET_DC_MOTOR_TYPE, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
		Serial.println(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }
	
    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;		
}

/*********************************************************************
*********************************************************************/

MMLower::Drive_RESULT MMLower::Set_Drive_Move_Func(int16_t power_left, int16_t power_right, uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[Set_Drive_MovePower]"));

    uint8_t data[5];
	
    BitConverter::GetBytes(data + 0, power_left);
	BitConverter::GetBytes(data + 2, power_right);
	data[4] = num - 1;
	
    CommSendData(COMM_CMD::SET_Drive_Move, data, 5);
    if (!WaitData(COMM_CMD::SET_Drive_Move, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR_POWER"));
        return Drive_RESULT::ERROR_MOTOR_POWER;
	}else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;
}

MMLower::Drive_RESULT MMLower::Set_Drive_Move_Degs(int16_t power_left, int16_t power_right, uint16_t Degree_c, bool brake, bool async, uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[Set_Drive_MoveDegs]"));

    uint8_t data[9];
	
    BitConverter::GetBytes(data + 0, power_left);
	BitConverter::GetBytes(data + 2, power_right);
	BitConverter::GetBytes(data + 4, Degree_c);
	data[6] = brake;
	data[7] = async;
	data[8] = num - 1;
	
    CommSendData(COMM_CMD::SET_Drive_MoveDegs, data, 9);
    if (!WaitData(COMM_CMD::SET_Drive_MoveDegs, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOVE_DEGS"));
        return Drive_RESULT::ERROR_MOTOR_POWER;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;
}

MMLower::Drive_RESULT MMLower::Set_Drive_Move_Time(int16_t power_left, int16_t power_right, uint32_t Time_mS, bool brake, bool async, uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[Set_Drive_MoveTime]"));

    uint8_t data[11];
	
    BitConverter::GetBytes(data + 0, power_left);
	BitConverter::GetBytes(data + 2, power_right);
	BitConverter::GetBytes(data + 4, Time_mS);
	data[8] = brake;
	data[9] = async;
	data[10] = num -1;
	
    CommSendData(COMM_CMD::SET_Drive_MoveTime, data, 11);
    if (!WaitData(COMM_CMD::SET_Drive_MoveTime, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR_TIME"));
        return Drive_RESULT::ERROR_MOTOR_POWER;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;
}

MMLower::Drive_RESULT MMLower::Set_Drive_MoveSync_Func(int16_t power_left, int16_t power_right, uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[Set_Drive_Move_SyncPower]"));

    uint8_t data[5];
	
    BitConverter::GetBytes(data + 0, power_left);
	BitConverter::GetBytes(data + 2, power_right);
	data[4] = num - 1;
	
    CommSendData(COMM_CMD::SET_Drive_MoveSync, data, 5);
    if (!WaitData(COMM_CMD::SET_Drive_MoveSync, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR_POWER"));
        return Drive_RESULT::ERROR_MOTOR_POWER;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;
}

MMLower::Drive_RESULT MMLower::Set_Drive_MoveSync_Degs(int16_t power_left, int16_t power_right, uint16_t Degree_c, bool brake, bool async, uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[SET_Drive_MoveSync_Degs]"));

    uint8_t data[9];
	
    BitConverter::GetBytes(data + 0, power_left);
	BitConverter::GetBytes(data + 2, power_right);
	BitConverter::GetBytes(data + 4, Degree_c);
	data[6] = brake;
	data[7] = async;
	data[8] = num - 1;
	
    CommSendData(COMM_CMD::SET_Drive_MoveSyncDegs, data, 9);
    if (!WaitData(COMM_CMD::SET_Drive_MoveSyncDegs, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOVE_DEGS"));
        return Drive_RESULT::ERROR_MOTOR_POWER;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;
}

MMLower::Drive_RESULT MMLower::Set_Drive_MoveSync_Time(int16_t power_left, int16_t power_right, uint32_t Time_mS, bool brake, bool async, uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[SET_Drive_MoveSync_Time]"));

    uint8_t data[11];
	
    BitConverter::GetBytes(data + 0, power_left);
	BitConverter::GetBytes(data + 2, power_right);
	BitConverter::GetBytes(data + 4, Time_mS);
	data[8] = brake;
	data[9] = async;
	data[10] = num -1;
	
    CommSendData(COMM_CMD::SET_Drive_MoveSyncTime, data, 11);
    if (!WaitData(COMM_CMD::SET_Drive_MoveSyncTime, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR_TIME"));
        return Drive_RESULT::ERROR_MOTOR_POWER;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;
}

MMLower::Drive_RESULT MMLower::Set_Drive_MoveGyro_Func(int16_t power, int16_t Target_dri, uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[SET_Drive_MoveGyro]"));

    uint8_t data[5];
	
    BitConverter::GetBytes(data + 0, power);
	BitConverter::GetBytes(data + 2, Target_dri);
	data[4] = num - 1;
	
    CommSendData(COMM_CMD::SET_Drive_Gyro, data, 5);
    if (!WaitData(COMM_CMD::SET_Drive_Gyro, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR_POWER"));
        return Drive_RESULT::ERROR_MOTOR_POWER;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;
}

MMLower::Drive_RESULT MMLower::Set_Drive_MoveGyro_Degs(int16_t power, int16_t Target_dri, uint16_t Degree_c, bool brake, bool async, uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[SET_Drive_MoveGyro_Degs]"));

    uint8_t data[9];
	
    BitConverter::GetBytes(data + 0, power);
	BitConverter::GetBytes(data + 2, Target_dri);
	BitConverter::GetBytes(data + 4, Degree_c);
	data[6] = brake;
	data[7] = async;
	data[8] = num - 1;
	
    CommSendData(COMM_CMD::SET_Drive_GyroDegs, data, 9);
    if (!WaitData(COMM_CMD::SET_Drive_GyroDegs, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOVE_DEGS"));
        return Drive_RESULT::ERROR_MOTOR_POWER;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;
}

MMLower::Drive_RESULT MMLower::Set_Drive_MoveGyro_Time(int16_t power, int16_t Target_dri, uint32_t Time_mS, bool brake, bool async, uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[SET_Drive_MoveGyro_Time]"));

    uint8_t data[11];
	
    BitConverter::GetBytes(data + 0, power);
	BitConverter::GetBytes(data + 2, Target_dri);
	BitConverter::GetBytes(data + 4, Time_mS);
	data[8] = brake;
	data[9] = async;
	data[10] = num -1;
	
    CommSendData(COMM_CMD::SET_Drive_GyroTime, data, 11);
    if (!WaitData(COMM_CMD::SET_Drive_GyroTime, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR_TIME"));
        return Drive_RESULT::ERROR_MOTOR_POWER;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;
}

MMLower::Drive_RESULT MMLower::Set_Drive_TurnGyro(int16_t power, int16_t Target_dri, uint8_t mode, bool brake, bool async, uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[SET_Drive_MoveGyro_Time]"));

    uint8_t data[10];
	
    BitConverter::GetBytes(data + 0, power);
	BitConverter::GetBytes(data + 2, Target_dri);
	data[4] = mode;
	data[5] = brake;
	data[6] = async;
	data[7] = num -1;
	
    CommSendData(COMM_CMD::SET_Drive_Turn, data, 8);
    if (!WaitData(COMM_CMD::SET_Drive_Turn, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR_TIME"));
        return Drive_RESULT::ERROR_MOTOR_POWER;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;	
}

//=========================================//
//=========================================//
//=========================================//


MMLower::Drive_RESULT MMLower::Get_Drive_isTaskDone(uint8_t num, bool * isEnd)
{
    MR4_DEBUG_PRINT_HEADER(F("[GET_Task_Done_Status]"));

    uint8_t data[2];
	data[0] = 1;
	data[1] = num -1;
	
    CommSendData(COMM_CMD::GET_Task_Done_Status, data, 2);
    if (!WaitData(COMM_CMD::GET_Task_Done_Status, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }
	
	if(b[0] == 0x01){
		isEnd[0] = false;
	}else if(b[0] == 0x02){
		isEnd[0] = true;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}
    
    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return Drive_RESULT::OK;
}

MMLower::Drive_RESULT MMLower::Set_Drive_Brake(bool brake, uint8_t num)
{
    MR4_DEBUG_PRINT_HEADER(F("[Set_Drive_Brake]"));

    uint8_t data[3];

	data[0] = brake;
	data[1] = num -1;
	
    CommSendData(COMM_CMD::SET_Drive_Brake, data, 2);
    if (!WaitData(COMM_CMD::SET_Drive_Brake, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR_TIME"));
        return Drive_RESULT::ERROR_MOTOR_POWER;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;
}

MMLower::Drive_RESULT MMLower::Set_Drive_Reset_Count(uint8_t num, bool reset)
{
    MR4_DEBUG_PRINT_HEADER(F("[SET_DC_TWO_MOTOR_Reset_count]"));

    uint8_t data[3];

	data[0] = (uint8_t)reset;
	data[1] = num -1;
	
    CommSendData(COMM_CMD::SET_DC_TWO_MOTOR_Reset_count, data, 2);
    if (!WaitData(COMM_CMD::SET_DC_TWO_MOTOR_Reset_count, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[1];
    if (!CommReadData(b, 1)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }

    if (b[0] == 0x00) {
        MR4_DEBUG_PRINT_TAIL(F("OK"));
        return Drive_RESULT::OK;
    }else if (b[0] == 0x02) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_MOTOR_TIME"));
        return Drive_RESULT::ERROR_MOTOR_POWER;
    }else if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}

    MR4_DEBUG_PRINT_TAIL(F("ERROR"));
    return Drive_RESULT::ERROR;
}

MMLower::Drive_RESULT MMLower::Get_Drive_EncoderCounter(uint8_t num, int32_t& enCounter)
{
    MR4_DEBUG_PRINT_HEADER(F("[Get_Drive_EncoderCounter]"));
	
	uint8_t data[2];
	data[0] = num - 1;

	CommSendData(COMM_CMD::GET_Drive_Counter, data, 1);
    if (!WaitData(COMM_CMD::GET_Drive_Counter, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[4];
    if (!CommReadData(b, 4)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }
	
    if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}	
	
    enCounter = BitConverter::ToInt32(b, 0);

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return Drive_RESULT::OK;
}

MMLower::Drive_RESULT MMLower::Get_Drive_Degrees(uint8_t num, int32_t& Degs)
{
    MR4_DEBUG_PRINT_HEADER(F("[Get_Drive_Degrees]"));

	uint8_t data[2];
	data[0] = num - 1;

	CommSendData(COMM_CMD::GET_Drive_Degress, data, 1);
    if (!WaitData(COMM_CMD::GET_Drive_Degress, 100)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_WAIT_TIMEOUT"));
        return Drive_RESULT::ERROR_WAIT_TIMEOUT;
    }

    uint8_t b[4];
    if (!CommReadData(b, 4)) {
        MR4_DEBUG_PRINT_TAIL(F("ERROR_READ_TIMEOUT"));
        return Drive_RESULT::ERROR_READ_TIMEOUT;
    }
	
    if(b[0] == 0x07){
        MR4_DEBUG_PRINT_TAIL(F("Drive_No Defined"));
        return Drive_RESULT::ERROR_Drive_Define;		
	}	
	
    Degs = BitConverter::ToInt32(b, 0);

    MR4_DEBUG_PRINT_TAIL(F("OK"));
    return Drive_RESULT::OK;
}



//--------------------------------------------------------------//
//--------------------------------------------------------------//


void MMLower::loop(void)
{
    WaitData(COMM_CMD::NONE);
}

void MMLower::onBtnChg(BtnChgCallback callback)
{
    callbackFunc = callback;
}

void MMLower::CommSendData(COMM_CMD cmd, uint8_t* data, uint16_t size)
{
    uint8_t  arr[3 + size];
    uint8_t* ptr = arr;

    *ptr++ = MatrixR4_COMM_LEAD;
    *ptr++ = ((~MatrixR4_COMM_LEAD) & 0xFF);
    *ptr++ = (uint8_t)cmd;

    for (uint16_t i = 0; i < size; i++) {
        *ptr++ = data[i];
    }
    commSerial->write(arr, 3 + size);
    commSerial->flush();
}

void MMLower::CommSendData(COMM_CMD cmd, uint8_t data)
{
    uint8_t _data[1] = {data};
    CommSendData((COMM_CMD)cmd, _data, 1);
}

bool MMLower::CommReadData(uint8_t* data, uint16_t size, uint32_t timeout_ms)
{
    uint32_t timeout = millis() + timeout_ms;
    while (millis() <= timeout) {
        if (commSerial->available() >= size) {
            for (uint16_t i = 0; i < size; i++) {
                data[i] = commSerial->read();
            }
            return true;
        }
    }
    // Timeout
    // Clear Buffer
    while (commSerial->available() > 0) {
        commSerial->read();
    }
    return false;
}

bool MMLower::WaitData(COMM_CMD cmd, uint32_t timeout_ms)
{
    static COMM_STATE state = COMM_STATE::WAIT_LEAD;

    uint32_t timeout = millis() + timeout_ms;
    while (millis() <= timeout) {
        if (commSerial->available() <= 0) {
            continue;
        }
        switch (state) {
        case COMM_STATE::WAIT_LEAD:
        {
            uint8_t b = commSerial->read();
            if (b == MatrixR4_COMM_LEAD) {
                state = COMM_STATE::WAIT_NOT_LEAD;
            }
        } break;

        case COMM_STATE::WAIT_NOT_LEAD:
        {
            uint8_t b = commSerial->read();
            if (b == ((~MatrixR4_COMM_LEAD) & 0xFF))
                state = COMM_STATE::WAIT_CMD;
            else
                state = COMM_STATE::WAIT_LEAD;
        } break;

        case COMM_STATE::WAIT_CMD:
        {
            uint8_t b = commSerial->read();
            if (b == (uint8_t)cmd) {
                state = COMM_STATE::WAIT_LEAD;
                return true;
            } else {
                state = COMM_STATE::WAIT_LEAD;
                HandleCommand(b);
            }
        } break;

        case COMM_STATE::ERROR:
        {
            // TODO: Handle Error
            MR4_DEBUG_PRINTLN("COMM_STATE: ERROR");
            state = COMM_STATE::WAIT_LEAD;
        } break;

        default: state = COMM_STATE::WAIT_LEAD; break;
        }
    }
    // Timeout
    state = COMM_STATE::WAIT_LEAD;
    return false;
}

void MMLower::HandleCommand(uint8_t cmd)
{
    switch (cmd) {
    case (uint8_t)COMM_CMD::AUTO_SEND_BUTTON_STATE:
    {
        uint8_t b[2];
        if (CommReadData(b, 2)) {
            if (callbackFunc == NULL) break;
            if (b[0] < MatrixR4_BUTTON_NUM) {
                callbackFunc(b[0] + 1, (BTN_STATE)b[1]);
            }
        }
    } break;
    case (uint8_t)COMM_CMD::AUTO_SEND_ENCODER_COUNTER:
    {
        uint8_t b[8];
        if (CommReadData(b, 8)) {
            for (uint8_t i = 0; i < MatrixR4_ENCODER_NUM; i++) {
                enCounter[i] = BitConverter::ToInt16(b, i * 2);
            }
        }
    } break;
    case (uint8_t)COMM_CMD::AUTO_SEND_IMU_EULER:
    {
        uint8_t b[6];
        if (CommReadData(b, 6)) {}
    } break;
    case (uint8_t)COMM_CMD::AUTO_SEND_IMU_GYRO:
    {
        uint8_t b[6];
        if (CommReadData(b, 6)) {
            imuGyroX = BitConverter::ToInt16(b, 0) / 100.0f;
            imuGyroY = BitConverter::ToInt16(b, 2) / 100.0f;
            imuGyroZ = BitConverter::ToInt16(b, 4) / 100.0f;
        }
    } break;
    case (uint8_t)COMM_CMD::AUTO_SEND_IMU_ACC:
    {
        uint8_t b[6];
        if (CommReadData(b, 6)) {
            imuAccX = BitConverter::ToInt16(b, 0) / 1000.0f;
            imuAccY = BitConverter::ToInt16(b, 2) / 1000.0f;
            imuAccZ = BitConverter::ToInt16(b, 4) / 1000.0f;
        }
    } break;
    default: break;
    }
}

MMLower mmL(8, 9, 57600);
