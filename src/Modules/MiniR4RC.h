/**
 * @file MiniR4RC.h
 * @brief Handling MiniR4.RCn functions.
 * @author MATRIX Robotics
 */
#ifndef MINIR4RC_H
#define MINIR4RC_H

#include "MMLower.h"

/**
 * @brief A class for controlling a remote control (RC) servo.
 *
 * This template class provides methods to initialize and control a servo
 * using its ID.
 * 
 * Note: Once you call MiniR4.begin(), the RC will auto begin.
 *
 * @tparam ID The unique identifier for the servo.
 */
template<uint8_t ID> class MiniR4RC
{
public:
    MiniR4RC() { _id = ID; }

    /**
     * @brief Initializes the servo with a default angle range.
     *
     * Sets the angle range of the servo to 0 to 180 degrees.
     *
     * @return true if initialization was successful, false otherwise.
     */
    bool begin(void)
    {
        MMLower::RESULT result = mmL.SetServoAngleRange(_id, 0, 180);
        return (result == MMLower::RESULT::OK);
    }

    /**
     * @brief Sets the hardware direction of the servo.
     *
     * @param dir True for forward direction, false for reverse direction.
     * @return true if the direction was set successfully, false otherwise.
     */
    bool setHWDir(bool dir)
    {
        MMLower::DIR    _dir   = (dir) ? MMLower::DIR::FORWARD : MMLower::DIR::REVERSE;
        MMLower::RESULT result = mmL.SetServoDir(_id, _dir);
        return (result == MMLower::RESULT::OK);
    }

    /**
     * @brief Sets the angle of the servo.
     *
     * Note: For 360 servo control command will update in future release.
     * 
     * @param angle The desired angle to set (0-180).
     * @return true if the angle was set successfully, false otherwise.
     */
    bool setAngle(uint16_t angle)
    {
        MMLower::RESULT result = mmL.SetServoAngle(_id, angle);
        return (result == MMLower::RESULT::OK);
    }

private:
    uint8_t _id;
};

#endif   // MINIR4RC_H
