#ifndef MINIR4ENC_H
#define MINIR4ENC_H

#include "MMLower.h"

template<uint8_t ID> class MiniR4ENC
{
public:
    MiniR4ENC() { _id = ID; }

    bool setHWDir(bool dir)
    {
        MMLower::DIR    _dir   = (dir) ? MMLower::DIR::FORWARD : MMLower::DIR::REVERSE;
        MMLower::RESULT result = mmL.SetEncoderDir(_id, _dir);
        return (result == MMLower::RESULT::OK);
    }

    int16_t getCounter(void)
    {
        int16_t         speed  = 0;
        MMLower::RESULT result = mmL.GetEncoderCounter(_id, speed);
        return speed;
    }

    bool resetCounter(void)
    {
        MMLower::RESULT result = mmL.SetEncoderResetCounter(_id);
        return (result == MMLower::RESULT::OK);
    }

private:
    uint8_t _id;
};

#endif   // MINIR4ENC_H
