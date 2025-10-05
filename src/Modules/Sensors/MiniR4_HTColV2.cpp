/**
 * @file MiniR4HTColV2.cpp
 * @brief Handling HiTechnic NXT Color Sensor V2 functions for MiniR4.
 * @author https://github.com/a10036gt/Arduino-HTColV2, MATRIX Robotics
 */
#include "MiniR4_HTColV2.h"

float HTColV2::max3(float a, float b, float c)
{
    return ((a > b) ? (a > c ? a : c) : (b > c ? b : c));
}

float HTColV2::min3(float a, float b, float c)
{
    return ((a < b) ? (a < c ? a : c) : (b < c ? b : c));
}

void HTColV2::begin()
{    
    // Set default mode to RGB
    setMode(HTCOLV2_MODE_RGB);
    delay(5);
}

void HTColV2::setMode(uint8_t mode)
{
    i2cMUXSelect();
	i2cWriteData(mode);
    delay(1);
}

void HTColV2::updateRGBData()
{
    i2cMUXSelect();
    
    // Read color data
    _pWire->beginTransmission(HTCOLV2_ADDRESS);
    _pWire->write(HTCOLV2_DATA_REG);
    _pWire->endTransmission(true);
    
    _pWire->requestFrom(HTCOLV2_ADDRESS, 4);
    
    if (_pWire->available()) {
        for (int i = 0; i < 4; i++) {
            _colorData[i] = _pWire->read();
        }
    }    
}

void HTColV2::calculateHSV()
{
    // Convert RGB to HSV
    float r = _colorData[1] / 255.0; // R is at index 1
    float g = _colorData[2] / 255.0; // G is at index 2  
    float b = _colorData[3] / 255.0; // B is at index 3
    
    float cmax = max3(r, g, b); // maximum of r, g, b
    float cmin = min3(r, g, b); // minimum of r, g, b
    float diff = cmax - cmin;   // diff of cmax and cmin
    
    // Calculate Hue
    if (cmax == cmin) {
        _hsvData.hue = 0;
    } else if (cmax == r) {
        _hsvData.hue = fmod((60 * ((g - b) / diff) + 360), 360.0);
    } else if (cmax == g) {
        _hsvData.hue = fmod((60 * ((b - r) / diff) + 120), 360.0);
    } else if (cmax == b) {
        _hsvData.hue = fmod((60 * ((r - g) / diff) + 240), 360.0);
    }
    
    // Calculate Saturation
    if (cmax == 0) {
        _hsvData.saturation = 0;
    } else {
        _hsvData.saturation = (diff / cmax) * 100;
    }
    
    // Calculate Value/Brightness
    _hsvData.value = cmax * 100;
}

uint8_t HTColV2::getColorNumber()
{
    updateRGBData();
    return _colorData[0]; // Color number is at index 0
}

uint8_t HTColV2::getR()
{
    updateRGBData();
    return _colorData[1]; // Red is at index 1
}

uint8_t HTColV2::getG()
{
    updateRGBData();
    return _colorData[2]; // Green is at index 2
}

uint8_t HTColV2::getB()
{
    updateRGBData();
    return _colorData[3]; // Blue is at index 3
}

void HTColV2::getRGB(uint8_t &r, uint8_t &g, uint8_t &b)
{
    updateRGBData();
    r = _colorData[1];
    g = _colorData[2];
    b = _colorData[3];
}

float HTColV2::getH()
{
    updateRGBData(); 
	calculateHSV();
    return _hsvData.hue;
}

float HTColV2::getS()
{
    updateRGBData(); 
	calculateHSV();
    return _hsvData.saturation;
}

float HTColV2::getV()
{
    updateRGBData(); 
	calculateHSV();
    return _hsvData.value;
}

void HTColV2::getHSV(float &h, float &s, float &v)
{
    updateRGBData(); 
	calculateHSV();
    h = _hsvData.hue;
    s = _hsvData.saturation;
    v = _hsvData.value;
}


void HTColV2::i2cMUXSelect()
{
    if (_ch < 0)
        return; // no MUX
    _pWire->beginTransmission(ADDR_PCA954X);
    _pWire->write((1 << _ch));
    _pWire->endTransmission(true);
    delay(1);
}

void HTColV2::i2cWriteData(uint8_t data)
{
    i2cMUXSelect();
    _pWire->beginTransmission(HTCOLV2_ADDRESS);
    _pWire->write(data);
    _pWire->endTransmission(true);
}

uint32_t HTColV2::i2cReadData(int reg, int num)
{
    i2cMUXSelect();
    _pWire->beginTransmission(HTCOLV2_ADDRESS);
    _pWire->write(reg);
    _pWire->endTransmission(true);

    delay(1);

    _pWire->requestFrom(HTCOLV2_ADDRESS, num);
    delay(1);

    uint32_t dataBuf = 0;
    int i = 0;

    while (_pWire->available() && i < num) {
        dataBuf = dataBuf << 8;
        dataBuf += _pWire->read();
        i++;
    }
    return dataBuf;
}