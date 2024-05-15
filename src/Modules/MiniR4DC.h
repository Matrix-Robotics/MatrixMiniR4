#ifndef MINIR4DC_H
#define MINIR4DC_H

#include "MMLower.h"

template<uint8_t ID> class MiniR4DC
{
public:
    MiniR4DC() { _id = ID; }

    bool begin(void)
    {
        MMLower::RESULT result = mmL.SetDCMotorSpeedRange(_id, 0, 100);
        return (result == MMLower::RESULT::OK);
    }

    bool setHWDir(bool dir)
    {
        MMLower::DIR    _dir   = (dir) ? MMLower::DIR::FORWARD : MMLower::DIR::REVERSE;
        MMLower::RESULT result = mmL.SetDCMotorDir(_id, _dir);
        return (result == MMLower::RESULT::OK);
    }

    bool setSpeed(uint16_t speed, bool dir)
    {
        MMLower::DIR    _dir   = (dir) ? MMLower::DIR::FORWARD : MMLower::DIR::REVERSE;
        MMLower::RESULT result = mmL.SetDCMotorSpeed(_id, speed, _dir);
        return (result == MMLower::RESULT::OK);
    }

private:
    uint8_t _id;
};

#endif   // MINIR4DC_H
