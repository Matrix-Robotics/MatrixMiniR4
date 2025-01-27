/**
 * @file MiniR4_DS18B20.h
 * @brief Lightweight library for working with 1-Wire Dallas DS18B20 thermometers.
 *
 * This library provides functions to work with DS18B20 temperature sensors using the 1-Wire protocol.
 * It supports reading the temperature, setting resolution, and handling CRC checks.
 * 
 * Add: Merge into MiniR4.Dn instance.
 * 
 * @author Egor 'Nich1con' Zakharov & AlexGyver
 * @license MIT
 */

#ifndef MINIR4_DS18B20_H
#define MINIR4_DS18B20_H

#include <Arduino.h>

#define DS_PROGMEM 1

#ifndef DS_CHECK_CRC
#define DS_CHECK_CRC true // true/false - проверка контрольной суммы принятых данных - надежнее, но тратит немного больше flash
#endif

#ifndef DS_CRC_USE_TABLE
#define DS_CRC_USE_TABLE false // true/false - использовать готовую таблицу контрольной суммы - значительно быстрее, +256 байт flash
#endif

#ifdef __AVR__
#define MOW_CLI() uint8_t oldSreg = SREG; cli();
#define MOW_SEI() SREG = oldSreg
#else
#define MOW_CLI()
#define MOW_SEI()
#endif

// ====================== CRC TABLE ======================
#if (DS_CRC_USE_TABLE == true)
static const uint8_t PROGMEM _ds_crc8_table[] = {
    0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
    0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
    0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
    0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
    0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
    0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
    0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
    0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
    0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
    0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
    0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
    0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
    0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
    0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
    0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
    0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35
};
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
static uint8_t _empDsAddr[1] = {1};
#pragma GCC diagnostic pop
#define DS_ADDR_MODE _empDsAddr

/**
 * @class MiniR4DS18B20
 * @brief Template class for interfacing with DS18B20 temperature sensors.
 * 
 * This class allows interfacing with DS18B20 sensors using the 1-Wire protocol. It supports multiple sensors
 * on the same line, addressable mode, and CRC checks.
 * 
 * @tparam PIN1 Unused pin (for future use).
 * @tparam PIN2 The GPIO pin number where the sensor is connected.
 * @tparam DS_ADDR Pointer to the sensor's address.
 * @tparam DS_AM Number of sensors on the line.
 * @tparam DS_PGM If true, addresses are stored in PROGMEM.
 */
template <uint8_t PIN1, uint8_t PIN2, uint8_t * DS_ADDR = (uint8_t * ) nullptr, uint8_t DS_AM = 1, bool DS_PGM = 0> class MiniR4DS18B20 {
  public:

    MiniR4DS18B20() {
      DS_PIN = PIN2; //PIN設定在DI_R位置，通過MiniR4Digital_h模板Pin1_2順序來初始化Grove或MATRIX物件
      pinMode(DS_PIN, INPUT);
      digitalWrite(DS_PIN, LOW);
    }
	
    /**
     * @brief Sets the resolution for a specific sensor.
     * @param res Resolution in bits (9-12).
     * @param idx Index of the sensor (default: 0).
     */
    void setResolution(uint8_t res, uint8_t idx = 0) {
      if (!oneWire_reset(DS_PIN)) return; // Проверка присутствия
      addressRoutine(idx); // Процедура адресации
      oneWire_write(0x4E, DS_PIN); // Запись RAM
      oneWire_write(0xFF, DS_PIN); // Максимум в верхний регистр тревоги
      oneWire_write(0x00, DS_PIN); // Минимум в верхний регистр тревоги
      oneWire_write(((constrain(res, 9, 12) - 9) << 5) | 0x1F, DS_PIN); // Запись конфигурации разрешения
    }

    /**
     * @brief Sets the resolution for all sensors on the line.
     * @param res Resolution in bits (9-12).
     */
    void setResolutionAll(uint8_t res) {
      for (int i = 0; i < DS_AM; i++) setResolution(res, i);
    }

    /**
     * @brief Set the address of the sensor.
     * @param addr Pointer to the sensor's address.
     */
    void setAddress(uint8_t * addr) {
      _addr = addr;
    }

    /**
     * @brief Reads the unique address of the sensor.
     * @param addr Pointer to the array where the address will be stored.
     * @return true if the address was successfully read, false otherwise.
     */
    bool readAddress(uint8_t * addr) {
      if (!oneWire_reset(DS_PIN)) return 0; // Проверка присутствия
      oneWire_write(0x33, DS_PIN); // Запрос адреса
      uint16_t sum = 0; // контрольная сумма
      uint8_t crc = 0; // обнуляем crc
      for (uint8_t i = 0; i < 8; i++) { // Прочитать 8 байт адреса
        addr[i] = oneWire_read(DS_PIN); // Записать в массив
        sum += addr[i]; // контрольная сумма
        #if(DS_CHECK_CRC == true)
        _ds_crc8_upd(crc, addr[i]); // Обновить значение CRC8
        #endif
      }
      return !(sum == 0x8F7 || !sum || crc); // CRC не сошелся или адрес нулевой - ошибка
    }

    /**
     * @brief Requests a temperature conversion from a specific sensor.
     * @param idx Index of the sensor (default: 0).
     */
    void requestTemp(uint8_t idx = 0) {
      state[idx] = 0; // запрошена новая температура
      if (!oneWire_reset(DS_PIN)) return; // Проверка присутствия
      addressRoutine(idx); // Процедура адресации
      oneWire_write(0x44, DS_PIN); // Запросить преобразование
    }

    /**
     * @brief Requests a temperature conversion from all sensors on the line.
     */
    void requestTempAll() {
      for (int i = 0; i < DS_AM; i++) requestTemp(i);
    }

    /**
     * @brief Gets the temperature as a float from a specific sensor.
     * @param idx Index of the sensor (default: 0).
     * @return Temperature in Celsius as a float.
     */
    float getTemp(uint8_t idx = 0) {
      if (!state[idx]) readTemp(idx);
      return (_buf[idx] / 16.0);
    }
	
	  /**
     * @brief Combines request and get temperature operations.
     * @param idx Index of the sensor (default: 0).
     * @return Temperature in Celsius as a float.
     */
    float requestAndGetTemp(uint8_t idx = 0) {
	    requestTemp(idx);
	    delay(1);
      if (!state[idx]) readTemp(idx);
      return (_buf[idx] / 16.0);
    }

    /**
     * @brief Gets the temperature as an integer from a specific sensor.
     * @param idx Index of the sensor (default: 0).
     * @return Temperature in Celsius as an integer.
     */
    int16_t getTempInt(uint8_t idx = 0) {
      if (!state[idx]) readTemp(idx);
      return (_buf[idx] >> 4);
    }

    /**
     * @brief Gets the raw temperature data from a specific sensor.
     * @param idx Index of the sensor (default: 0).
     * @return Raw temperature data.
     */
    int16_t getRaw(uint8_t idx = 0) {
      if (!state[idx]) readTemp(idx);
      return _buf[idx];
    }

    /**
     * @brief Reads the temperature from a specific sensor.
     * @param idx Index of the sensor (default: 0).
     * @return true if the temperature was successfully read, false otherwise.
     */
    bool readTemp(uint8_t idx = 0) {
      state[idx] = 1;
      if (!oneWire_reset(DS_PIN)) return 0; // датчик оффлайн
      addressRoutine(idx); // Процедура адресации
      oneWire_write(0xBE, DS_PIN); // Запросить температуру
      uint8_t crc = 0; // обнуляем crc
      int16_t temp; // переменная для расчёта температуры
      uint16_t sum = 0; // контрольная сумма
      for (uint8_t i = 0; i < 9; i++) { // Считать RAM
        uint8_t data = oneWire_read(DS_PIN); // Прочитать данные
        sum += data;
        #if(DS_CHECK_CRC == true)
        _ds_crc8_upd(crc, data); // Обновить значение CRC8
        #endif
        if (i == 0) temp = data;
        else if (i == 1) temp |= (data << 8);
      }
      if (sum == 0x8F7 || !sum || crc) return 0; // датчик оффлайн или данные повреждены        
      if (temp != 0x0550) _buf[idx] = temp; // пропускаем первое чтение (85 градусов)
      return 1;
    }

    /**
     * @brief Checks if the sensor is online.
     * @param idx Index of the sensor (default: 0). THE LINE MUST BE PULLED UP
     * @return true if the sensor is online, false otherwise.
     */ 
    bool online(uint8_t idx = 0) {
      if (DS_ADDR != nullptr) {
        if (!oneWire_reset(DS_PIN)) return 0;
        addressRoutine(idx);
        oneWire_write(0xBE, DS_PIN);
        uint16_t sum = 0;
        for (uint8_t i = 0; i < 5; i++) sum += oneWire_read(DS_PIN);
        return (sum != 0x4FB);
      } else return oneWire_reset(DS_PIN);
    }

  private: 
	int DS_PIN; // Pin number for the DS18B20 sensor.
	bool state[DS_AM];
    int16_t _buf[DS_AM];
    uint8_t * _addr = DS_ADDR;
	
	void addressRoutine(uint8_t idx) { // Addressing procedure
      if (DS_ADDR != nullptr) { // Адрес определен?
        oneWire_write(0x55, DS_PIN); // Говорим термометрам слушать адрес
        for (uint8_t i = 0; i < 8; i++) {
          if (DS_PGM) oneWire_write(pgm_read_byte( & _addr[i + idx * 8]), DS_PIN);
          else oneWire_write(_addr[i + idx * 8], DS_PIN);
        }
      } else oneWire_write(0xCC, DS_PIN); // Адреса нет - пропускаем адресацию на линии
    }

    bool oneWire_reset(uint8_t pin) {
      pinMode(pin, OUTPUT);
      delayMicroseconds(600);
      pinMode(pin, INPUT);
      MOW_CLI();
      delayMicroseconds(60);
      bool pulse = digitalRead(pin);
      MOW_SEI();
      delayMicroseconds(600);
      return !pulse;
    }

    void oneWire_write(uint8_t data, uint8_t pin) {
      for (uint8_t i = 8; i; i--) {
        pinMode(pin, OUTPUT);
        MOW_CLI();
        if (data & 1) {
          delayMicroseconds(5);
          pinMode(pin, INPUT);
          delayMicroseconds(60);
        } else {
          delayMicroseconds(60);
          pinMode(pin, INPUT);
          delayMicroseconds(5);
        }
        MOW_SEI();
        data >>= 1;
      }
    }

    uint8_t oneWire_read(uint8_t pin) {
      uint8_t data = 0;
      for (uint8_t i = 8; i; i--) {
        data >>= 1;
        MOW_CLI();
        pinMode(pin, OUTPUT);
        delayMicroseconds(2);
        pinMode(pin, INPUT);
        delayMicroseconds(8);
        if (digitalRead(pin)) data |= (1 << 7);
        delayMicroseconds(60);
        MOW_SEI();
      }
      return data;
    }

    void _ds_crc8_upd(uint8_t & crc, uint8_t data) {
      #if(DS_CRC_USE_TABLE == true) // Используем таблицу?
      crc = pgm_read_byte( & _ds_crc8_table[crc ^ data]); // Тогда берем готовое значение
      #else // считаем вручную
      #ifdef __AVR__
      // резкий алгоритм для AVR
      uint8_t counter;
      uint8_t buffer;
      asm volatile(
        "EOR %[crc_out], %[data_in] \n\t"
        "LDI %[counter], 8          \n\t"
        "LDI %[buffer], 0x8C        \n\t"
        "_loop_start_%=:            \n\t"
        "LSR %[crc_out]             \n\t"
        "BRCC _loop_end_%=          \n\t"
        "EOR %[crc_out], %[buffer]  \n\t"
        "_loop_end_%=:              \n\t"
        "DEC %[counter]             \n\t"
        "BRNE _loop_start_%=": [crc_out]
        "=r"(crc), [counter]
        "=d"(counter), [buffer]
        "=d"(buffer): [crc_in]
        "0"(crc), [data_in]
        "r"(data)
      );
      #else
      // обычный для всех остальных
      uint8_t i = 8;
      while (i--) {
        crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
        data >>= 1;
      }
      #endif
      #endif
    }
};

#endif // MINIR4_DS18B20_H
