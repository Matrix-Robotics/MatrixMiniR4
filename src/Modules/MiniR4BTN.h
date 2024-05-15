#ifndef MINIR4BTN_H
#define MINIR4BTN_H

#include "MMLower.h"

template<uint8_t ID> class MiniR4BTN
{
public:
    MiniR4BTN() { _id = ID; }

    bool getState(void)
    {
        bool            state  = false;
        MMLower::RESULT result = mmL.GetButtonState(_id, state);
        return state;
    }

private:
    uint8_t _id;
};

#endif   // MINIR4BTN_H
