/**
 * @file MiniR4I2C.h
 * @brief Handling MiniR4.I2Cn functions.
 * @author MATRIX Robotics
 */

#ifndef MINIR4I2C_H
#define MINIR4I2C_H

#include "Sensors/MiniR4ColorSensorExt.h"
#include "Sensors/MiniR4LaserSensorExt.h"
#include "Sensors/MiniR4MxCtrlExt.h"

#include "Sensors/MiniR4_MXLaserV2.h"
#include "Sensors/MiniR4_MXColorV3.h"
#include "Sensors/MiniR4_MXGesture.h"


#include "Sensors/MiniR4_GroveI2C_BME280.h"
// #include "Sensors/MiniR4_EIOBoardExt.h"


/**
 * @brief Template class for managing I2C devices.
 *
 * This class handles multiple I2C devices associated with the same ID.
 *
 * @tparam ID Unique identifier for the I2C devices.
 * @tparam WIRE Pointer to the TwoWire object for I2C communication.
 */

template<uint8_t ID, TwoWire* WIRE> class MiniR4I2C
{
public:
    /**
     * @brief Constructor initializes the I2C devices with the given ID and wire.
     *
     * This constructor sets the channel and wire for all associated devices.
     */
    MiniR4I2C()
    {
        // MXMotion._ch = ID;
        MXLaser._ch  = ID;
        MXLaserV2._ch  = ID;
        MXColor._ch  = ID;
        MXColorV3._ch  = ID;
        MXGesture._ch  = ID;
        MXCtrl._ch  = ID;
        GroveBME280._ch  = ID;
        // EIOBoard._ch  = ID;

        // MXMotion._pWire = WIRE; ///< Matrix Motion sensor
        MXLaser._pWire  = WIRE; ///< Matrix Laser sensor
        MXLaserV2._pWire  = WIRE; ///< Matrix Laser V2 ToF sensor
        MXColor._pWire  = WIRE; ///< Matrix Color sensor
        MXColorV3._pWire  = WIRE; ///< Matrix Color sensor V3
        MXGesture._pWire  = WIRE; ///< Matrix Gesture sensor
        MXCtrl._pWire  = WIRE; ///< Matrix Controller (HT)
        GroveBME280._pWire  = WIRE; ///< Grove BME280 sensor
        // EIOBoard._pWire  = WIRE; ///< Matrix EIOBoard
    }

    // MatrixMotion MXMotion; ///< Matrix Motion sensor instance.
    MatrixLaser  MXLaser; ///< Matrix Laser sensor instance.
    MatrixLaserV2  MXLaserV2; ///< Matrix Laser V2 ToF sensor instance.
    MatrixColor  MXColor; ///< Matrix Color sensor instance.
    MatrixColorV3  MXColorV3; ///< Matrix Color sensor V3 instance.
    MatrixGesture  MXGesture; ///< Matrix Gesture sensor instance.
    MatrixController  MXCtrl; ///< Matrix Controller (HT) instance.
    GroveI2C_BME280  GroveBME280; ///< Grove BME280 sensor instance.
    // MatrixEIOBoard  EIOBoard; ///< Matrix EIOBoard instance.

    // Adafruit_TCS34725 MXColor = Adafruit_TCS34725(
    //     TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X, TCS34725_ADDRESS, WIRE, ID);

private:
};

#endif   // MINIR4I2C_H
