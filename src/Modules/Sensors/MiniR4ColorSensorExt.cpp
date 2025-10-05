/**
 * @file MiniR4ColorSensorExt.cpp
 * @brief Matrix Color Sensor functions.
 * @author MATRIX Robotics
 */
#include "MiniR4ColorSensorExt.h"

bool MatrixColor::begin()
{
    _pWire->begin();
    i2cMUXSelect();
    delay(50);
    if (i2cReadData(Device_ID) == 0x43) {
        i2cWriteData(Device_CONFIG, 0x10);   // reset
        delay(500);
        i2cWriteData(Device_CONFIG, setting);   // enable
        return true;
    } else {
        return false;
    }
}

void MatrixColor::setGamma(bool state)
{
    i2cMUXSelect();
    if (state) {
        setting |= 0b00000100;
    } else {
        setting &= 0b00001011;
    }
    i2cWriteData(Device_CONFIG, setting);
}

void MatrixColor::setLight(bool state, bool mode, uint8_t pwm)
{
    i2cMUXSelect();

    if (state) {
        setting |= 0b00000010;
    } else {
        setting &= 0b00001101;
    }
    if (mode) {
        setting |= 0b00000001;
    } else {
        setting &= 0b00001110;
    }
    i2cWriteData(Device_CONFIG, setting);
    i2cWriteData(Device_LIGHT, pwm);
}


uint8_t MatrixColor::getColor(ColorType color)
{
    i2cMUXSelect();
    return i2cReadData(ColorRegType(color));
}


uint8_t MatrixColor::getGrayscale()
{
    return i2cReadData(Device_GRAY);
}

uint8_t MatrixColor::getColorNumber_Deprecated()
{
    i2cMUXSelect();
    return i2cReadData(Device_NUM_COLOR);
}

void MatrixColor::RGB2HSV(uint8_t r, uint8_t g, uint8_t b, float &h, float &s, float &v) {
    float rd = r / 255.0;
    float gd = g / 255.0;
    float bd = b / 255.0;

    float maxVal = max(rd, max(gd, bd));
    float minVal = min(rd, min(gd, bd));

    v = maxVal * 100.0;  // 轉換為 0-100%
    float delta = maxVal - minVal;

    if (delta < 0.00001) {
        s = 0;
        h = 0;
        return;
    }
    if (maxVal > 0.0) {
        s = (delta / maxVal) * 100.0;  // 轉換為 0-100%
    } else {
        s = 0;
        h = 0;
        return;
    }
    if (rd >= maxVal) h = (gd - bd) / delta; 
    else if (gd >= maxVal) h = 2.0 + (bd - rd) / delta; 
    else h = 4.0 + (rd - gd) / delta; 
    h *= 60.0;
    
    if (h < 0.0) h += 360.0;
}

void MatrixColor::updateHSV() {
    i2cMUXSelect();
    uint8_t r = i2cReadData(Device_RED);
    uint8_t g = i2cReadData(Device_GREEN);
    uint8_t b = i2cReadData(Device_BLUE);
    
    float h, s, v;
    RGB2HSV(r, g, b, h, s, v);
    
    _cachedH = h;          // 0-360°
    _cachedS = s / 100.0;  // 轉回 0-1 內部儲存
    _cachedV = v / 100.0;  // 轉回 0-1 內部儲存
}

float MatrixColor::getH() {
    updateHSV();
    return _cachedH;  // 0-360 度
}

float MatrixColor::getS() {
    updateHSV();
    return _cachedS * 100.0;  // 轉換為 0-100%
}

float MatrixColor::getV() {
    updateHSV();
    return _cachedV * 100.0;  // 轉換為 0-100%
}

int8_t MatrixColor::getColorID() {
    i2cMUXSelect();
    uint8_t r = i2cReadData(Device_RED);
    uint8_t g = i2cReadData(Device_GREEN);
    uint8_t b = i2cReadData(Device_BLUE);
    uint8_t gray = i2cReadData(Device_GRAY);
    
    // 使用灰階值判斷是否太暗
    if (gray < 10) {
		if ((r+g+b) > 5){
			return COLOR_BLACK;
		}else{	
			return COLOR_NONE;
		}
    }
    
    float h, s, v;
    RGB2HSV(r, g, b, h, s, v);
    
    // 低飽和度 -> 黑白判斷
    if (s < 35.0) {
        if (gray > 115) return COLOR_WHITE;
        if (gray < 115) return COLOR_BLACK;
        return COLOR_NONE;
    }
    
    if (h < 0) return COLOR_NONE;
    
    // 色相判斷
    if (h >= 355.0 || h < 15.0) return COLOR_RED;
    if (h >= 270.0 && h < 355.0) return COLOR_VIOLET;
    if (h >= 170.0 && h < 270.0) return COLOR_BLUE;
    // if (h >= 170.0 && h < 220.0) return COLOR_LIGHTBLUE;
    if (h >= 90.0 && h < 170.0) return COLOR_GREEN;
    if (h >= 45.0 && h < 90.0) return COLOR_YELLOW;
    if (h >= 15.0 && h < 45.0) {
        return (s > 50.0) ? COLOR_RED : COLOR_YELLOW;
    }
    
    return COLOR_NONE;
}

uint8_t MatrixColor::i2cReadData(ColorRegType reg)
{

    _pWire->beginTransmission(MatrixColor_ADDR);
    _pWire->write(reg);
    _pWire->endTransmission(1);

    delay(1);

    _pWire->requestFrom(MatrixColor_ADDR, 1);

    delay(1);

    return _pWire->read();
}

void MatrixColor::i2cMUXSelect()
{
    if (_ch < 0) return;   // no MUX
    _pWire->beginTransmission(ADDR_PCA954X);
    _pWire->write((1 << _ch));
    _pWire->endTransmission(1);
    delayMicroseconds(300);
}

void MatrixColor::i2cWriteData(ColorRegType reg, uint8_t data)
{

    _pWire->beginTransmission(MatrixColor_ADDR);

    _pWire->write(reg);
    _pWire->write(data);

    _pWire->endTransmission(1);
}