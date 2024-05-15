#ifndef MiniR4I2CDevice_H
#define MiniR4I2CDevice_H

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#    define ADDR_PCA954X 0x70
#endif

///< The class which defines how we will talk to this device over I2C
class MiniR4_I2CDevice
{
public:
    MiniR4_I2CDevice(uint8_t addr, TwoWire* theWire = &Wire, uint8_t mux_ch = -1);
    uint8_t address(void);
    bool    begin(bool addr_detect = true);
    void    end(void);
    bool    detected(void);

    bool read(uint8_t* buffer, size_t len, bool stop = true);
    bool write(
        const uint8_t* buffer, size_t len, bool stop = true, const uint8_t* prefix_buffer = nullptr,
        size_t prefix_len = 0);
    bool write_then_read(
        const uint8_t* write_buffer, size_t write_len, uint8_t* read_buffer, size_t read_len,
        bool stop = false);
    bool setSpeed(uint32_t desiredclk);

    /*!   @brief  How many bytes we can read in a transaction
     *    @return The size of the Wire receive/transmit buffer */
    size_t maxBufferSize() { return _maxBufferSize; }

    uint8_t _ch = 0;

private:
    uint8_t  _addr;
    TwoWire* _wire;
    bool     _begun;
    size_t   _maxBufferSize;
    bool     _read(uint8_t* buffer, size_t len, bool stop);
    void     i2cMUXSelect(void);
};

#endif   // MiniR4I2CDevice_H
