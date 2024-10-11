/**
 * @file MiniR4BTN.h
 * @brief Handling the MiniR4.BTNn functions.
 * @author MATRIX Robotics
 */

#ifndef MINIR4BTN_H
#define MINIR4BTN_H

#include "MMLower.h"

/**
 * @brief A class template for handling button state retrieval for a specific button ID.
 *
 * This class interacts with the MMLower library to get the state of a button
 * identified by a unique ID.
 *
 * @tparam ID The unique identifier for the button.
 */
template <uint8_t ID>
class MiniR4BTN
{
public:
    MiniR4BTN() { _id = ID; }

    /**
     * @brief Retrieves the current state of the button.
     * 
     * This method calls the MMLower library to check the state of the button
     * identified by the unique ID.
     * 
     * @return true if the button is pressed; false otherwise.
     */
    bool getState(void)
    {
        bool state = false;
        MMLower::RESULT result = mmL.GetButtonState(_id, state);
        return state;
    }

private:
    uint8_t _id;
};

#endif // MINIR4BTN_H
