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

#include "Sensors/MiniR4_HTColV2.h"
#include "Sensors/MiniR4_MXLineTracer.h"

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
        MXLaser._ch  = ID;
        MXLaserV2._ch  = ID;
        MXColor._ch  = ID;
        MXColorV3._ch  = ID;
        MXGesture._ch  = ID;
        MXCtrl._ch  = ID;
        GroveBME280._ch  = ID;
        HTCol._ch  = ID;
        MXLineTracer._ch  = ID;

        MXLaser._pWire  = WIRE; ///< Matrix Laser sensor
        MXLaserV2._pWire  = WIRE; ///< Matrix Laser V2 ToF sensor
        MXColor._pWire  = WIRE; ///< Matrix Color sensor
        MXColorV3._pWire  = WIRE; ///< Matrix Color sensor V3
        MXGesture._pWire  = WIRE; ///< Matrix Gesture sensor
        MXCtrl._pWire  = WIRE; ///< Matrix Controller (HT)
        GroveBME280._pWire  = WIRE; ///< Grove BME280 sensor
		HTCol._pWire  = WIRE; ///< HT Color V2 sensor
		MXLineTracer._pWire  = WIRE; ///< MXLineTracer sensor
    }

    MatrixLaser  MXLaser; ///< Matrix Laser sensor instance.
    MatrixLaserV2  MXLaserV2; ///< Matrix Laser V2 ToF sensor instance.
    MatrixColor  MXColor; ///< Matrix Color sensor instance.
    MatrixColorV3  MXColorV3; ///< Matrix Color sensor V3 instance.
    MatrixGesture  MXGesture; ///< Matrix Gesture sensor instance.
    MatrixController  MXCtrl; ///< Matrix Controller (HT) instance.
    GroveI2C_BME280  GroveBME280; ///< Grove BME280 sensor instance.
    HTColV2  HTCol; ///< HT Color V2 sensor instance.
    MatrixLineTracer  MXLineTracer; ///< MatrixLineTracer sensor instance.
    

private:
};

#endif   // MINIR4I2C_H
