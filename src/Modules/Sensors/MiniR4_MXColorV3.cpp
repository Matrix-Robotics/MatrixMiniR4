/**
 * @file MiniR4_MXColorV3.cpp
 * @brief A library for interfacing with the MATRIX Color Seneor V3 sensor (TCS34725) via I2C.
 * 
 * This class allows reading color data from the MATRIX Color Seneor V3 
 * 
 * Add: Merge into MiniR4.I2C instance.
 * 
 * @author MATRIX Robotics
 * @version 1.0
 * @date 2024
 * @license MIT License
 */
#include "MiniR4_MXColorV3.h"

bool MatrixColorV3::begin() {
  // 检查传感器ID
  _pWire->begin();
  i2cMUXSelect();
  _pWire->beginTransmission(TCS34725_ADDRESS);
  _pWire->write(TCS34725_COMMAND_BIT | TCS34725_ID);
  _pWire->endTransmission();
  _pWire->requestFrom(TCS34725_ADDRESS, 1);
  if (_pWire->read() != 0x44) { // TCS34725的默认ID值
    return false;
  }

  // 配置集成时间和增益
  writeRegister(TCS34725_ATIME, TCS34725_INTEGRATIONTIME_50MS);
  writeRegister(TCS34725_CONTROL, TCS34725_GAIN_1X);

  // 启用传感器
  writeRegister(TCS34725_ENABLE, TCS34725_ENABLE_PON);
  delay(3); // 启动延迟
  writeRegister(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
  
  return true; // 初始化成功
}

uint16_t MatrixColorV3::getR(){
  return readRegister16(TCS34725_RDATAL);
}

uint16_t MatrixColorV3::getG(){
  return readRegister16(TCS34725_GDATAL);
}

uint16_t MatrixColorV3::getB(){
  return readRegister16(TCS34725_BDATAL);
}

uint16_t MatrixColorV3::getC(){
  return readRegister16(TCS34725_CDATAL);
}

float MatrixColorV3::calcColorTemp(uint16_t r, uint16_t g, uint16_t b) {
  if (r == 0 || g == 0 || b == 0) return 0; // 避免零值导致的问题

  float X = (-0.14282 * r) + (1.54924 * g) + (-0.95641 * b);
  float Y = (-0.32466 * r) + (1.57837 * g) + (-0.73191 * b);
  float Z = (-0.68202 * r) + (0.77073 * g) + (0.56332 * b);

  if (X + Y + Z == 0) return 0; // 避免除以零

  float x = X / (X + Y + Z);
  float y = Y / (X + Y + Z);

  // CIE 1931 2° 标准观察者公式
  float n = (x - 0.3320) / (0.1858 - y);
  float colorTemp = (449.0 * pow(n, 3)) + (3525.0 * pow(n, 2)) + (6823.3 * n) + 5520.33;

  return colorTemp;
}

float MatrixColorV3::calcLux(uint16_t r, uint16_t g, uint16_t b) {
  return (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
}

void MatrixColorV3::RGB2HSV(uint16_t r, uint16_t g, uint16_t b, float &h, float &s, float &v) {
  float rd = r / 255.0;
  float gd = g / 255.0;
  float bd = b / 255.0;

  float maxVal = max(rd, max(gd, bd));
  float minVal = min(rd, min(gd, bd));

  v = maxVal;
  float delta = maxVal - minVal;

  if (delta < 0.00001) {
    s = 0;
    h = 0; // undefined, maybe nan?
    return;
  }
  if (maxVal > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
    s = (delta / maxVal);
  } else {
    // if max is 0, then r = g = b = 0              
    // s = 0, v is undefined
    s = 0;
    h = NAN; // its now undefined
    return;
  }
  if (rd >= maxVal) h = (gd - bd) / delta; 
  else if (gd >= maxVal) h = 2.0 + (bd - rd) / delta; 
  else h = 4.0 + (rd - gd) / delta; 
  h *= 60.  ;
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
  delayMicroseconds(100);
}