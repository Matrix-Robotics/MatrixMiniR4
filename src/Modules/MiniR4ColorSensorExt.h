#ifndef _MiniR4ColorSensorExt_H_
#define _MiniR4ColorSensorExt_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#    define ADDR_PCA954X 0x70
#endif

#define MatrixColor_ADDR 0x22

typedef enum __ColorType
{
    R = 4,
    G,
    B,
    C,
    M,
    Y,
    K
} ColorType;

class MatrixColor
{
private:
    typedef enum __ColorRegType
    {
        Device_ID = 1,
        Device_CONFIG,
        Device_LIGHT,
        Device_RED,
        Device_GREEN,
        Device_BLUE,
        Device_CYAN,
        Device_MAGENTA,
        Device_YELLOW,
        Device_BLACK,
        Device_GRAY,
        Device_NUM_COLOR
    } ColorRegType;

    uint8_t setting = 0x0F;
    uint8_t i2cReadData(ColorRegType reg);
    void    i2cMUXSelect();
    void    i2cWriteData(ColorRegType reg, uint8_t data);

public:
    uint8_t  _ch = 0;
    TwoWire* _pWire;

    bool    begin();
    void    setGamma(bool state);
    void    setLight(bool state, bool mode, uint8_t pwm);
    uint8_t getColor(ColorType color);
    uint8_t getGrayscale();
    uint8_t getColorNumber();
};

#endif
