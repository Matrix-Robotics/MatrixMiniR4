#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <_ansi.h>
#include <stddef.h>

#include "ssd1306_conf.h"

#if defined(STM32F0)
#    include "stm32f0xx_hal.h"
#elif defined(STM32F1)
#    include "stm32f1xx_hal.h"
#elif defined(STM32F4)
#    include "stm32f4xx_hal.h"
#    include "stm32f4xx_hal_gpio.h"
#elif defined(STM32L0)
#    include "stm32l0xx_hal.h"
#elif defined(STM32L1)
#    include "stm32l1xx_hal.h"
#elif defined(STM32L4)
#    include "stm32l4xx_hal.h"
#elif defined(STM32L5)
#    include "stm32l5xx_hal.h"
#elif defined(STM32F3)
#    include "stm32f3xx_hal.h"
#elif defined(STM32H7)
#    include "stm32h7xx_hal.h"
#elif defined(STM32F7)
#    include "stm32f7xx_hal.h"
#elif defined(STM32G0)
#    include "stm32g0xx_hal.h"
#elif defined(STM32G4)
#    include "stm32g4xx_hal.h"
#else
#    error \
        "SSD1306 library was tested only on STM32F0, STM32F1, STM32F3, STM32F4, STM32F7, STM32L0, STM32L1, STM32L4, STM32H7, STM32G0, STM32G4 MCU families. Please modify ssd1306.h if you know what you are doing. Also please send a pull request if it turns out the library works on other MCU's as well!"
#endif

#ifdef SSD1306_X_OFFSET
#    define SSD1306_X_OFFSET_LOWER (SSD1306_X_OFFSET & 0x0F)
#    define SSD1306_X_OFFSET_UPPER ((SSD1306_X_OFFSET >> 4) & 0x07)
#else
#    define SSD1306_X_OFFSET_LOWER 0
#    define SSD1306_X_OFFSET_UPPER 0
#endif

#include "ssd1306_fonts.h"

/* vvv I2C config vvv */

#ifndef SSD1306_I2C_PORT
#    define SSD1306_I2C_PORT hi2c1
#endif

#ifndef SSD1306_I2C_ADDR
#    define SSD1306_I2C_ADDR (0x3C << 1)
#endif

/* ^^^ I2C config ^^^ */

/* vvv SPI config vvv */

#ifndef SSD1306_SPI_PORT
#    define SSD1306_SPI_PORT hspi2
#endif

#ifndef SSD1306_CS_Port
#    define SSD1306_CS_Port GPIOB
#endif
#ifndef SSD1306_CS_Pin
#    define SSD1306_CS_Pin GPIO_PIN_12
#endif

#ifndef SSD1306_DC_Port
#    define SSD1306_DC_Port GPIOB
#endif
#ifndef SSD1306_DC_Pin
#    define SSD1306_DC_Pin GPIO_PIN_14
#endif

#ifndef SSD1306_Reset_Port
#    define SSD1306_Reset_Port GPIOA
#endif
#ifndef SSD1306_Reset_Pin
#    define SSD1306_Reset_Pin GPIO_PIN_8
#endif

/* ^^^ SPI config ^^^ */

#if defined(SSD1306_USE_I2C)
extern I2C_HandleTypeDef SSD1306_I2C_PORT;
#elif defined(SSD1306_USE_SPI)
extern SPI_HandleTypeDef SSD1306_SPI_PORT;
#else
#    error "You should define SSD1306_USE_SPI or SSD1306_USE_I2C macro!"
#endif

// SSD1306 OLED height in pixels
#ifndef SSD1306_HEIGHT
#    define SSD1306_HEIGHT 64
#endif

// SSD1306 width in pixels
#ifndef SSD1306_WIDTH
#    define SSD1306_WIDTH 128
#endif

#ifndef SSD1306_BUFFER_SIZE
#    define SSD1306_BUFFER_SIZE SSD1306_WIDTH* SSD1306_HEIGHT / 8
#endif

_BEGIN_STD_C

// Enumeration for screen colors
typedef enum
{
    Black = 0x00,   // Black color, no pixel
    White = 0x01    // Pixel is set. Color depends on OLED
} SSD1306_COLOR;

typedef enum
{
    SSD1306_OK  = 0x00,
    SSD1306_ERR = 0x01   // Generic error.
} SSD1306_Error_t;

// Struct to store transformations
typedef struct
{
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t  Initialized;
    uint8_t  DisplayOn;
} SSD1306_t;

typedef struct
{
    uint8_t x;
    uint8_t y;
} SSD1306_VERTEX;

// Procedure definitions

_END_STD_C

#endif   // __SSD1306_H__
