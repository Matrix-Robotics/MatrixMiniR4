/**
 * @file MiniR4LED.h
 * @brief Handling MiniR4.LED functions.
 * @author MATRIX Robotics
 */

#ifndef MINIR4LED_H
#define MINIR4LED_H

/**
 * @brief Class for controlling an RGB LED.
 *
 * This class provides functionality to set colors and brightness for an RGB LED
 * connected to a specified pin. It uses the WS2812B protocol for communication.
 */
class MiniR4LED
{
public:
    MiniR4LED() {}

    /**
     * @brief Initializes the LED. (Built-in at Uno Pin 7)
     *
     * @param pin The pin number to which the LED is connected.
     */
    void begin(uint8_t pin)
    {
        port_table[0] = R_PORT0;
        port_table[1] = R_PORT1;
        port_table[2] = R_PORT2;
        port_table[3] = R_PORT3;
        port_table[4] = R_PORT4;
        port_table[5] = R_PORT5;
        port_table[6] = R_PORT6;
        port_table[7] = R_PORT7;

        for (uint8_t i = 0; i < 16; i++) {
            mask_table[i] = 1 << i;
        }

        _brightness[0] = 255;
        _brightness[1] = 255;

        _pin = pin;
        pinMode(_pin, OUTPUT);
        digitalWriteFast(_pin, 0);
    }

    /**
     * @brief Sets the color of the LED.
     *
     * @param idx The index of the LED (1 or 2).
     * @param rgb The color in RGB format.
     * @return True if the color was set successfully, false otherwise.
     */
    bool setColor(uint8_t idx, uint32_t rgb)
    {
        if (idx < 1 || idx > 2) {
            return false;
        }
        _leds[idx - 1] = rgb;
        Update();
        return true;
    }

    /**
     * @brief Sets the color of the LED using separate RGB values.
     *
     * @param idx The index of the LED (1 or 2).
     * @param r Red value (0-255).
     * @param g Green value (0-255).
     * @param b Blue value (0-255).
     * @return True if the color was set successfully, false otherwise.
     */
    bool setColor(uint8_t idx, uint8_t r, uint8_t g, uint8_t b)
    {
        uint32_t rgb = (r << 16) | (g << 8) | b;
        return setColor(idx, rgb);
    }

    /**
     * @brief Sets the brightness of the LED.
     *
     * @param idx The index of the LED (1 or 2).
     * @param brightness The brightness level (0-255). (Default is 255)
     */
    void setBrightness(uint8_t idx, uint8_t brightness)
    {
        if (idx < 1 || idx > 2) {
            return;
        }
        _brightness[idx - 1] = brightness;
        Update();
    }

private:
    uint8_t  _pin;
    uint32_t _leds[2];
    uint8_t  _brightness[2];

    R_PORT0_Type* port_table[8];
    uint16_t      mask_table[16];

    void Update(void)
    {
        uint8_t r, g, b;
        r = ((_leds[0] >> 16) & 0xFF) * _brightness[0] / 255;
        g = ((_leds[0] >> 8) & 0xFF) * _brightness[0] / 255;
        b = (_leds[0] & 0xFF) * _brightness[0] / 255;
        ProcessWS2812BProtocol(r, g, b);
        r = ((_leds[1] >> 16) & 0xFF) * _brightness[1] / 255;
        g = ((_leds[1] >> 8) & 0xFF) * _brightness[1] / 255;
        b = (_leds[1] & 0xFF) * _brightness[1] / 255;
        ProcessWS2812BProtocol(r, g, b);
        delayMicroseconds(500);
    }

    inline void digitalWriteFast(uint8_t pin, uint8_t val) __attribute__((always_inline, unused))
    {
        if (val) {
            port_table[g_pin_cfg[pin].pin >> 8]->POSR = mask_table[g_pin_cfg[pin].pin & 0xff];
        } else {
            port_table[g_pin_cfg[pin].pin >> 8]->PORR = mask_table[g_pin_cfg[pin].pin & 0xff];
        }
    }

    void ProcessWS2812BProtocol(uint8_t r, uint8_t g, uint8_t b)
    {
        volatile byte i, tempByte, m_Byte_R, m_Byte_G, m_Byte_B;

        m_Byte_R = r;
        m_Byte_G = g;
        m_Byte_B = b;

        noInterrupts();

        // Color: G
        for (i = 0; i < 8; i++) {
            digitalWriteFast(_pin, 1);

            if ((m_Byte_G & 0x80) == 0) {
                // asm("nop;");
                digitalWriteFast(_pin, 0);
                m_Byte_G <<= 1;
                tempByte >>= 1;
            } else {
                for (uint32_t i = 0; i < 5; i++) {
                    asm("nop;");
                }
                m_Byte_G <<= 1;
                tempByte <<= 1;
                digitalWriteFast(_pin, 0);
            }
        }

        // Color: R
        for (i = 0; i < 8; i++) {
            digitalWriteFast(_pin, 1);

            if ((m_Byte_R & 0x80) == 0) {
                // asm("nop;");
                digitalWriteFast(_pin, 0);
                m_Byte_R <<= 1;
                tempByte >>= 1;
            } else {
                for (uint32_t i = 0; i < 5; i++) {
                    asm("nop;");
                }
                m_Byte_R <<= 1;
                tempByte <<= 1;
                digitalWriteFast(_pin, 0);
            }
        }

        // Color: B
        for (i = 0; i < 8; i++) {
            digitalWriteFast(_pin, 1);

            if ((m_Byte_B & 0x80) == 0) {
                // asm("nop;");
                digitalWriteFast(_pin, 0);
                m_Byte_B <<= 1;
                tempByte >>= 1;
            } else {
                for (uint32_t i = 0; i < 5; i++) {
                    asm("nop;");
                }
                m_Byte_B <<= 1;
                tempByte <<= 1;
                digitalWriteFast(_pin, 0);
            }
        }

        interrupts();
    }
};

#endif   // MINIR4LED_H
