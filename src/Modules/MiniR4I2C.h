#ifndef MINIR4I2C_H
#define MINIR4I2C_H

#include "MiniR4ColorSensorExt.h"
#include "MiniR4LaserSensorExt.h"
#include "MiniR4MxCtrlExt.h"
#include "MiniR4MotionExt.h"
#include "MiniR4TCS34725.h"
#include "MiniR4_GroveI2C_BME280.h"

template<uint8_t ID, TwoWire* WIRE> class MiniR4I2C
{
public:
    MiniR4I2C()
    {
        MXMotion._ch = ID;
        MXLaser._ch  = ID;
        MXColor._ch  = ID;
        MXCtrl._ch  = ID;
        GroveBME280._ch  = ID;

        MXMotion._pWire = WIRE;
        MXLaser._pWire  = WIRE;
        MXColor._pWire  = WIRE;
        MXCtrl._pWire  = WIRE;
        GroveBME280._pWire  = WIRE;
    }

    MatrixMotion MXMotion;
    MatrixLaser  MXLaser;
    MatrixColor  MXColor;
    MatrixController  MXCtrl;
    GroveI2C_BME280  GroveBME280;

    // Adafruit_TCS34725 MXColor = Adafruit_TCS34725(
    //     TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X, TCS34725_ADDRESS, WIRE, ID);

private:
};

#endif   // MINIR4I2C_H
