/**
 * @file MiniR4_MXLineTracer.cpp
 * @brief A library for interfacing with the MATRIX Line Tracer 10CH sensor via I2C.
 * 
 * @author MATRIX Robotics
 * @version 1.1
 * @date 2025
 * @license MIT License
 */
#include "MiniR4_MXLineTracer.h"

void MatrixLineTracer::begin() {
  _pWire->begin();
  for (int i = 0; i < 10; i++) {
    _sensorCache[i] = 0;
  }
  
  _sensorWeights[0] = -4.5;
  _sensorWeights[1] = -3.5;
  _sensorWeights[2] = -2.5;
  _sensorWeights[3] = -1.5;
  _sensorWeights[4] = -0.5;
  _sensorWeights[5] = 0.5;
  _sensorWeights[6] = 1.5;
  _sensorWeights[7] = 2.5;
  _sensorWeights[8] = 3.5;
  _sensorWeights[9] = 4.5;
  
  _threshold = 30;
}

uint8_t MatrixLineTracer::read(byte cmd) {
  i2cMUXSelect();
  
  _pWire->beginTransmission(MXLINE_ADDRESS);
  _pWire->write(cmd);
  _pWire->endTransmission();
  _pWire->requestFrom(MXLINE_ADDRESS, (uint8_t)1);
  return _pWire->available() ? _pWire->read() : 0;
}

bool MatrixLineTracer::getAllSensors(uint8_t data[10]) {
  i2cMUXSelect();
  _pWire->beginTransmission(MXLINE_ADDRESS);
  _pWire->write(0x00);
  _pWire->endTransmission();
  _pWire->requestFrom(MXLINE_ADDRESS, (uint8_t)10);
  
  for (int i = 0; i < 10; i++) {
    if (!_pWire->available()) return false;
    data[i] = _pWire->read();
  }
  delayMicroseconds(500);
  return true;
}

uint8_t MatrixLineTracer::getSensor(uint8_t n) {
  if (n < 1 || n > 10) return 0;
  getAllSensors(_sensorCache);
  return _sensorCache[n - 1];
}

String MatrixLineTracer::getVersion() {
  i2cMUXSelect();
  _pWire->beginTransmission(MXLINE_ADDRESS);
  _pWire->write(0xF0);
  _pWire->endTransmission();
  _pWire->requestFrom(MXLINE_ADDRESS, (uint8_t)5);
  String v = "";
  while (_pWire->available()) v += (char)_pWire->read();
  return v;
}

uint8_t MatrixLineTracer::getLineWidth() { 
  return read(0x10); 
}

// int8_t MatrixLineTracer::getError() { 
  // return (int8_t)read(0x11); 
// }

float MatrixLineTracer::getError() {
  uint8_t sensors[10];
  getAllSensors(sensors);
  
  float weightedSum = 0.0;
  float totalWeight = 0.0;
  
  for (int i = 0; i < 10; i++) {
    float lineIntensity = 100 - sensors[i];
    if (lineIntensity > _threshold) {
      weightedSum += lineIntensity * _sensorWeights[i];
      totalWeight += lineIntensity;
    }
  }
  
  if (totalWeight == 0) {
    return 0;  // Offline
  }
  
  return weightedSum / totalWeight;
}

bool MatrixLineTracer::isOnline() {
	// return read(0x12);
    return (read(0x10) != 0) ? true : false;
}

uint8_t MatrixLineTracer::getLastSensor() { 
  return read(0x13); 
}

uint8_t MatrixLineTracer::getJunctionType() { 
  return read(0x15); 
}

void MatrixLineTracer::setWeights(float w1, float w2, float w3, float w4, float w5, 
                                   float w6, float w7, float w8, float w9, float w10) {
  _sensorWeights[0] = w1;
  _sensorWeights[1] = w2;
  _sensorWeights[2] = w3;
  _sensorWeights[3] = w4;
  _sensorWeights[4] = w5;
  _sensorWeights[5] = w6;
  _sensorWeights[6] = w7;
  _sensorWeights[7] = w8;
  _sensorWeights[8] = w9;
  _sensorWeights[9] = w10;
}

void MatrixLineTracer::setThreshold(uint8_t threshold) {
  _threshold = threshold;
}

void MatrixLineTracer::startCalibration() {
  i2cMUXSelect();
  _pWire->beginTransmission(MXLINE_ADDRESS);
  _pWire->write(0x20);
  _pWire->endTransmission();
}

void MatrixLineTracer::endCalibration() {
  i2cMUXSelect();
  _pWire->beginTransmission(MXLINE_ADDRESS);
  _pWire->write(0x21);
  _pWire->endTransmission();
}

void MatrixLineTracer::i2cMUXSelect()
{
  // Due to some HW issue, currently not support I2C MUX.
  // delayMicroseconds(500);
  if (_ch < 0){
	delayMicroseconds(500);
    return; // no MUX
  }
  _pWire->beginTransmission(ADDR_PCA954X);
  _pWire->write((1 << _ch));
  _pWire->endTransmission(1);
  delayMicroseconds(100);
}