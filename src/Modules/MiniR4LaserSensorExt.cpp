#include "MiniR4LaserSensorExt.h"

bool MatrixLaser::begin()
{
    _pWire->begin();
    i2cMUXSelect();
    delay(50);
    if (i2cReadData(Device_ID) == 0x47) {
        i2cWriteData(Device_CONFIG, 0x04);   // reset
        delay(500);
        i2cWriteData(Device_CONFIG, 0x02);   // enable
        return true;
    } else {
        return false;
    }
}

uint16_t MatrixLaser::getDistance()
{
    i2cMUXSelect();
    if (((i2cReadData(Device_CONFIG) & 0x01) == 0)) {
        uint16_t data = (uint16_t)(i2cReadData(Distance_H) << 8 | i2cReadData(Distance_L));
        return data;
    } else {
        return 8191;
    }
}

uint8_t MatrixLaser::i2cReadData(LaserRegType reg)
{

    _pWire->beginTransmission(MatrixLaser_ADDR);
    _pWire->write(reg);
    _pWire->endTransmission(1);

    delay(1);

    _pWire->requestFrom(MatrixLaser_ADDR, 1);

    delay(1);

    return _pWire->read();
}

void MatrixLaser::i2cMUXSelect()
{
    if (_ch < 0) return;   // no MUX
    _pWire->beginTransmission(ADDR_PCA954X);
    _pWire->write((1 << _ch));
    _pWire->endTransmission(1);
    delayMicroseconds(300);
}

void MatrixLaser::i2cWriteData(LaserRegType reg, uint8_t data)
{

    _pWire->beginTransmission(MatrixLaser_ADDR);

    _pWire->write(reg);
    _pWire->write(data);

    _pWire->endTransmission(1);
}
