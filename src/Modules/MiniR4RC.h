#ifndef MINIR4RC_H
#define MINIR4RC_H

#include "MMLower.h"

template<uint8_t ID> class MiniR4RC
{
public:
    MiniR4RC() { _id = ID; }

    bool begin(void)
    {
        MMLower::RESULT result = mmL.SetServoAngleRange(_id, 0, 180);
        return (result == MMLower::RESULT::OK);
    }

    bool setHWDir(bool dir)
    {
        MMLower::DIR    _dir   = (dir) ? MMLower::DIR::FORWARD : MMLower::DIR::REVERSE;
        MMLower::RESULT result = mmL.SetServoDir(_id, _dir);
        return (result == MMLower::RESULT::OK);
    }

    bool setAngle(uint16_t angle)
    {
        MMLower::RESULT result = mmL.SetServoAngle(_id, angle);
        return (result == MMLower::RESULT::OK);
    }

private:
    uint8_t _id;
};

#endif   // MINIR4RC_H
