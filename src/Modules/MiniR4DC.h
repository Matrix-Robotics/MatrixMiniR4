#ifndef MINIR4DC_H
#define MINIR4DC_H

#include "MMLower.h"

template<uint8_t ID> class MiniR4DC
{
public:
    MiniR4DC() { _id = ID; }

    bool begin(void)
    {
        MMLower::RESULT result  = mmL.SetDCMotorSpeedRange(_id, 0, 100);
        MMLower::RESULT result1 = mmL.SetEncoderResetCounter(_id);
        MMLower::RESULT result2 = mmL.SetDCMotorPower(_id, 0);

        return (
            result == MMLower::RESULT::OK && result1 == MMLower::RESULT::OK &&
            result2 == MMLower::RESULT::OK);
    }

    bool setReverse(bool dir)
    {
        MMLower::DIR    _dir        = (dir) ? MMLower::DIR::REVERSE : MMLower::DIR::FORWARD;
        MMLower::RESULT resultMotor = mmL.SetDCMotorDir(_id, _dir);
        // MMLower::RESULT resultEnc   = mmL.SetEncoderDir(_id, _dir);
        // return (resultMotor == MMLower::RESULT::OK && resultEnc == MMLower::RESULT::OK);
        return (resultMotor == MMLower::RESULT::OK);
    }

    bool setPower(int16_t power)
    {
        MMLower::RESULT result = mmL.SetDCMotorPower(_id, power);
        return (result == MMLower::RESULT::OK);
    }

    bool setSpeed(int16_t speed)
    {
        MMLower::RESULT result = mmL.SetDCMotorSpeed(_id, speed);
        return (result == MMLower::RESULT::OK);
    }

    bool rotateFor(int16_t power, uint16_t degree)
    {
        MMLower::RESULT result = mmL.SetDCMotorRotate(_id, power, degree);
        return (result == MMLower::RESULT::OK);
    }

    bool setFixSpeedPID(float kp, float ki, float kd)
    {
        MMLower::RESULT result = mmL.SetPIDParam(_id, 0, kp, ki, kd);
        return (result == MMLower::RESULT::OK);
    }

    bool setRotatePID(float kp, float ki, float kd)
    {
        MMLower::RESULT result = mmL.SetPIDParam(_id, 1, kp, ki, kd);
        return (result == MMLower::RESULT::OK);
    }

    int32_t getCounter(void)
    {
        int32_t         counter = 0;
        MMLower::RESULT result  = mmL.GetEncoderCounter(_id, counter);
        return counter;
    }

    bool resetCounter(void)
    {
        MMLower::RESULT result = mmL.SetEncoderResetCounter(_id);
        return (result == MMLower::RESULT::OK);
    }

private:
    uint8_t _id;
};

#endif   // MINIR4DC_H
