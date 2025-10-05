/**
 * @file MiniR4_MXColorV3.cpp
 * @brief A library for interfacing with the MATRIX Color Seneor V3 sensor (TCS34725) via I2C.
 * 
 * This class allows reading color data from the MATRIX Color Seneor V3 
 * 
 * Add: Merge into MiniR4.I2C instance.
 * 
 * @author MATRIX Robotics
 * @version 1.1
 * @date 2025
 * @license MIT License
 */
#include "MiniR4_MXColorV3.h"

bool MatrixColorV3::begin() {
  _pWire->begin();
  i2cMUXSelect();
  _pWire->beginTransmission(TCS34725_ADDRESS);
  _pWire->write(TCS34725_COMMAND_BIT | TCS34725_ID);
  _pWire->endTransmission();
  _pWire->requestFrom(TCS34725_ADDRESS, 1);
  if (_pWire->read() != 0x44) {
    return false;
  }

  writeRegister(TCS34725_ATIME, TCS34725_INTEGRATIONTIME_50MS);
  writeRegister(TCS34725_CONTROL, TCS34725_GAIN_1X);

  writeRegister(TCS34725_ENABLE, TCS34725_ENABLE_PON);
  delay(3);
  writeRegister(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
  
  return true;
}

void MatrixColorV3::setWhiteBalance(uint16_t white_r, uint16_t white_g, uint16_t white_b) {
    uint16_t maxVal = max(white_r, max(white_g, white_b));
    if (maxVal == 0) return;
    
    _wb_R = (float)maxVal / white_r;
    _wb_G = (float)maxVal / white_g;
    _wb_B = (float)maxVal / white_b;
}

// 原始 16-bit 讀取函數
uint16_t MatrixColorV3::getRaw_R(){
  return readRegister16(TCS34725_RDATAL);
}

uint16_t MatrixColorV3::getRaw_G(){
  return readRegister16(TCS34725_GDATAL);
}

uint16_t MatrixColorV3::getRaw_B(){
  return readRegister16(TCS34725_BDATAL);
}

// 正規化的 RGB 讀取函數（0-255）
int16_t MatrixColorV3::getR(){
    uint16_t r = readRegister16(TCS34725_RDATAL) * _wb_R;
    uint16_t c = getC();
    if (c < 20) return 0;
    uint32_t normalized = ((uint32_t)r * 255) / c;
    return (normalized > 255) ? 255 : (int16_t)normalized;
}

int16_t MatrixColorV3::getG(){
    uint16_t g = readRegister16(TCS34725_GDATAL) * _wb_G;
    uint16_t c = getC();
    if (c < 20) return 0;
    uint32_t normalized = ((uint32_t)g * 255) / c;
    return (normalized > 255) ? 255 : (int16_t)normalized;
}

int16_t MatrixColorV3::getB(){
    uint16_t b = readRegister16(TCS34725_BDATAL) * _wb_B;
    uint16_t c = getC();
    if (c < 20) return 0;
    uint32_t normalized = ((uint32_t)b * 255) / c;
    return (normalized > 255) ? 255 : (int16_t)normalized;
}

int16_t MatrixColorV3::getC(){
  return readRegister16(TCS34725_CDATAL);
}

void MatrixColorV3::readNormalizedRGB(int16_t &r, int16_t &g, int16_t &b, int16_t &c) {
    uint16_t raw_r = readRegister16(TCS34725_RDATAL) * _wb_R;
    uint16_t raw_g = readRegister16(TCS34725_GDATAL) * _wb_G;
    uint16_t raw_b = readRegister16(TCS34725_BDATAL) * _wb_B;
    uint16_t raw_c = readRegister16(TCS34725_CDATAL);
    
    c = raw_c;
    
    if (raw_c < 20) {
        r = g = b = 0;
    } else {
        r = min(255, (int16_t)(((uint32_t)raw_r * 255) / raw_c));
        g = min(255, (int16_t)(((uint32_t)raw_g * 255) / raw_c));
        b = min(255, (int16_t)(((uint32_t)raw_b * 255) / raw_c));
    }
}

void MatrixColorV3::RGB2HSV(uint16_t r, uint16_t g, uint16_t b, float &h, float &s, float &v) {
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
  if (maxVal > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
    s = (delta / maxVal) * 100.0;  // 轉換為 0-100%
  } else {
    // if max is 0, then r = g = b = 0              
    // s = 0, v is undefined
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

void MatrixColorV3::updateHSV() {
  int16_t r, g, b, c;
  readNormalizedRGB(r, g, b, c);
  float h, s, v;
  RGB2HSV(r, g, b, h, s, v);
  
  _cachedH = h;          // 0-360°
  _cachedS = s / 100.0;  // 轉回 0-1 內部儲存
  _cachedV = v / 100.0;  // 轉回 0-1 內部儲存
}

float MatrixColorV3::getH() {
  updateHSV();
  return _cachedH;  // 0-360 度
}

float MatrixColorV3::getS() {
  updateHSV();
  return _cachedS * 100.0;  // 轉換為 0-100%
}

float MatrixColorV3::getV() {
  updateHSV();
  return _cachedV * 100.0;  // 轉換為 0-100%
}

int8_t MatrixColorV3::getColorID() {
  int16_t r, g, b, c;
  readNormalizedRGB(r, g, b, c);
  
  if (c < 20) {
    return COLOR_NONE;
  }
  
  float h, s, v;
  RGB2HSV(r, g, b, h, s, v);
  
  if (s < 35.0) {
    if (c > 115.0) return COLOR_WHITE;
    if (c < 115.0) return COLOR_BLACK;
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

float MatrixColorV3::calcColorTemp(uint16_t r, uint16_t g, uint16_t b) {
  if (r == 0 || g == 0 || b == 0) return 0;

  float X = (-0.14282 * r) + (1.54924 * g) + (-0.95641 * b);
  float Y = (-0.32466 * r) + (1.57837 * g) + (-0.73191 * b);
  float Z = (-0.68202 * r) + (0.77073 * g) + (0.56332 * b);

  if (X + Y + Z == 0) return 0;

  float x = X / (X + Y + Z);
  float y = Y / (X + Y + Z);

  // CIE 1931
  float n = (x - 0.3320) / (0.1858 - y);
  float colorTemp = (449.0 * pow(n, 3)) + (3525.0 * pow(n, 2)) + (6823.3 * n) + 5520.33;

  return colorTemp;
}

float MatrixColorV3::calcLux(uint16_t r, uint16_t g, uint16_t b) {
  return (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
}

void MatrixColorV3::writeRegister(uint8_t reg, uint8_t value) {
  i2cMUXSelect();
  _pWire->beginTransmission(TCS34725_ADDRESS);
  _pWire->write(TCS34725_COMMAND_BIT | reg);
  _pWire->write(value);
  _pWire->endTransmission();
}

uint16_t MatrixColorV3::readRegister16(uint8_t reg) {
  i2cMUXSelect();
  _pWire->beginTransmission(TCS34725_ADDRESS);
  _pWire->write(TCS34725_COMMAND_BIT | reg);
  _pWire->endTransmission();

  _pWire->requestFrom(TCS34725_ADDRESS, 2);
  uint16_t value = _pWire->read();
  value |= (_pWire->read() << 8);
  return value;
}

void MatrixColorV3::i2cMUXSelect()
{
  if (_ch < 0)
    return; // no MUX
  _pWire->beginTransmission(ADDR_PCA954X);
  _pWire->write((1 << _ch));
  _pWire->endTransmission(1);
  delayMicroseconds(500);
}