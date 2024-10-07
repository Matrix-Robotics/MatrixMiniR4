/*
    Seeed_BME280.cpp
    Example sketch for bme280

    Copyright (c) 2016 seeed technology inc.
    Website    : www.seeedstudio.com
    Author     : Lambor
    Create Time:
    Change Log : Rework for combine into MATRIX Mini R4 IIC system.

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#include "MiniR4_GroveI2C_BME280.h"

bool GroveI2C_BME280::begin(int i2c_addr) {
  uint8_t retry = 0;
  uint8_t chip_id = 0;

  _devAddr = i2c_addr;
  _pWire -> begin();
  i2cMUXSelect();

  while ((retry++ < 5) && (chip_id != 0x60)) {
    chip_id = BME280Read8(BME280_REG_CHIPID);
    #ifdef BMP280_DEBUG_PRINT
    Serial.print("Read chip ID: ");
    Serial.println(chip_id);
    #endif
    delay(100);
  }
  if (chip_id != 0x60) {
    Serial.println("Read Chip ID fail!");
    return false;
  }

  dig_T1 = BME280Read16LE(BME280_REG_DIG_T1);
  dig_T2 = BME280ReadS16LE(BME280_REG_DIG_T2);
  dig_T3 = BME280ReadS16LE(BME280_REG_DIG_T3);

  dig_P1 = BME280Read16LE(BME280_REG_DIG_P1);
  dig_P2 = BME280ReadS16LE(BME280_REG_DIG_P2);
  dig_P3 = BME280ReadS16LE(BME280_REG_DIG_P3);
  dig_P4 = BME280ReadS16LE(BME280_REG_DIG_P4);
  dig_P5 = BME280ReadS16LE(BME280_REG_DIG_P5);
  dig_P6 = BME280ReadS16LE(BME280_REG_DIG_P6);
  dig_P7 = BME280ReadS16LE(BME280_REG_DIG_P7);
  dig_P8 = BME280ReadS16LE(BME280_REG_DIG_P8);
  dig_P9 = BME280ReadS16LE(BME280_REG_DIG_P9);

  dig_H1 = BME280Read8(BME280_REG_DIG_H1);
  dig_H2 = BME280Read16LE(BME280_REG_DIG_H2);
  dig_H3 = BME280Read8(BME280_REG_DIG_H3);
  dig_H4 = (BME280Read8(BME280_REG_DIG_H4) << 4) | (0x0F & BME280Read8(BME280_REG_DIG_H4 + 1));
  dig_H5 = (BME280Read8(BME280_REG_DIG_H5 + 1) << 4) | (0x0F & BME280Read8(BME280_REG_DIG_H5) >> 4);
  dig_H6 = (int8_t) BME280Read8(BME280_REG_DIG_H6);

  writeRegister(BME280_REG_CONTROLHUMID, 0x05); //Choose 16X oversampling
  writeRegister(BME280_REG_CONTROL, 0xB7); //Choose 16X oversampling

  return true;
}

float GroveI2C_BME280::getTemperature(void) {
  int32_t var1, var2;

  int32_t adc_T = BME280Read24(BME280_REG_TEMPDATA);
  // Check if the last transport successed
  if (!isTransport_OK) {
    return 0;
  }
  adc_T >>= 4;
  var1 = (((adc_T >> 3) - ((int32_t)(dig_T1 << 1))) *
    ((int32_t) dig_T2)) >> 11;

  var2 = (((((adc_T >> 4) - ((int32_t) dig_T1)) *
      ((adc_T >> 4) - ((int32_t) dig_T1))) >> 12) *
    ((int32_t) dig_T3)) >> 14;

  t_fine = var1 + var2;
  float T = (t_fine * 5 + 128) >> 8;
  return T / 100;
}

float GroveI2C_BME280::getPressure(void) {
  int64_t var1, var2, p;
  // Call getTemperature to get t_fine
  getTemperature();
  // Check if the last transport successed
  if (!isTransport_OK) {
    return 0;
  }
  int32_t adc_P = BME280Read24(BME280_REG_PRESSUREDATA);
  adc_P >>= 4;
  var1 = ((int64_t) t_fine) - 128000;
  var2 = var1 * var1 * (int64_t) dig_P6;
  var2 = var2 + ((var1 * (int64_t) dig_P5) << 17);
  var2 = var2 + (((int64_t) dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t) dig_P3) >> 8) + ((var1 * (int64_t) dig_P2) << 12);
  var1 = (((((int64_t) 1) << 47) + var1)) * ((int64_t) dig_P1) >> 33;
  if (var1 == 0) {
    return 0; // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t) dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t) dig_P8) * p) >> 19;
  p = ((p + var1 + var2) >> 8) + (((int64_t) dig_P7) << 4);
  return (float)(p / 256.0);
}

float GroveI2C_BME280::getHumidity(void) {
  int32_t v_x1_u32r, adc_H;
  // Call getTemperature to get t_fine
  getTemperature();
  // Check if the last transport successed
  if (!isTransport_OK) {
    return 0;
  }
  adc_H = BME280Read16(BME280_REG_HUMIDITYDATA);
  v_x1_u32r = (t_fine - ((int32_t) 76800));
  v_x1_u32r = (
    ((((adc_H << 14) - (((int32_t) dig_H4) << 20) - (((int32_t) dig_H5) * v_x1_u32r)) +
      ((int32_t) 16384)) >> 15) * (((((((v_x1_u32r * ((int32_t) dig_H6)) >> 10) *
        (((v_x1_u32r * ((int32_t) dig_H3)) >> 11) + ((int32_t) 32768))) >> 10) + ((int32_t) 2097152)) *
      ((int32_t) dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t) dig_H1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
  v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
  v_x1_u32r = v_x1_u32r >> 12;
  float h = v_x1_u32r / 1024.0;
  return h;
}

float GroveI2C_BME280::calcAltitude(float pressure) {
  if (!isTransport_OK) {
    return 0;
  }

  float A = pressure / 101325;
  float B = 1 / 5.25588;
  float C = pow(A, B);
  C = 1.0 - C;
  C = C / 0.0000225577;
  return C;
}

uint8_t GroveI2C_BME280::BME280Read8(uint8_t reg) {
  i2cMUXSelect();
  _pWire -> beginTransmission(_devAddr);
  _pWire -> write(reg);
  _pWire -> endTransmission();

  _pWire -> requestFrom(_devAddr, 1);
  // return 0 if slave didn't response
  if (_pWire -> available() < 1) {
    isTransport_OK = false;
    return 0;
  } else {
    isTransport_OK = true;
  }

  return _pWire -> read();
}

uint16_t GroveI2C_BME280::BME280Read16(uint8_t reg) {
  uint8_t msb, lsb;
  i2cMUXSelect();
  _pWire -> beginTransmission(_devAddr);
  _pWire -> write(reg);
  _pWire -> endTransmission();

  _pWire -> requestFrom(_devAddr, 2);
  // return 0 if slave didn't response
  if (_pWire -> available() < 2) {
    isTransport_OK = false;
    return 0;
  } else {
    isTransport_OK = true;
  }
  msb = _pWire -> read();
  lsb = _pWire -> read();

  return (uint16_t) msb << 8 | lsb;
}

uint16_t GroveI2C_BME280::BME280Read16LE(uint8_t reg) {
  uint16_t data = BME280Read16(reg);
  return (data >> 8) | (data << 8);
}

int16_t GroveI2C_BME280::BME280ReadS16(uint8_t reg) {
  return (int16_t) BME280Read16(reg);
}

int16_t GroveI2C_BME280::BME280ReadS16LE(uint8_t reg) {
  return (int16_t) BME280Read16LE(reg);
}

uint32_t GroveI2C_BME280::BME280Read24(uint8_t reg) {
  uint32_t data;
  i2cMUXSelect();
  _pWire -> beginTransmission(_devAddr);
  _pWire -> write(reg);
  _pWire -> endTransmission();

  _pWire -> requestFrom(_devAddr, 3);
  // return 0 if slave didn't response
  if (_pWire -> available() < 3) {
    isTransport_OK = false;
    return 0;
  } else if (isTransport_OK == false) {
    isTransport_OK = true;
    if (!begin(_devAddr)) {
      #ifdef BMP280_DEBUG_PRINT
      Serial.println("Device not connected or broken!");
      #endif
    }
  }
  data = _pWire -> read();
  data <<= 8;
  data |= _pWire -> read();
  data <<= 8;
  data |= _pWire -> read();

  return data;
}

void GroveI2C_BME280::writeRegister(uint8_t reg, uint8_t val) {
  i2cMUXSelect();
  _pWire -> beginTransmission(_devAddr); // start transmission to device
  _pWire -> write(reg); // send register address
  _pWire -> write(val); // send value to write
  _pWire -> endTransmission(); // end transmission
}

void GroveI2C_BME280::i2cMUXSelect() {
  if (_ch < 0) return; // no MUX
  _pWire -> beginTransmission(ADDR_PCA954X);
  _pWire -> write((1 << _ch));
  _pWire -> endTransmission(1);
  delayMicroseconds(300);
}