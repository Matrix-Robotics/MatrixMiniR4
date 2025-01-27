/**
 * @file MiniR4_MXGesture.h
 * @brief A library for interfacing with the MATRIX Gesture Seneor PAJ7620 via I2C.
 *
 * This class allows reading 9 gesture from the MATRIX Gesture Seneor.
 *
 * Based On DFRobot_PAJ7620U2 https://github.com/DFRobot/DFRobot_PAJ7620U2
 * Add: Merge into MiniR4.I2C instance.
 *
 * @author MATRIX Robotics
 * @version 1.0
 * @date 2025
 * @license MIT License
 */
#ifndef _MiniR4_MXGesture_H_
#define _MiniR4_MXGesture_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef ADDR_PCA954X
#define ADDR_PCA954X 0x70
#endif

// Open this macro to check the detailed running process of the program.
// #define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...)                     \
    {                                \
        Serial.print("[");           \
        Serial.print(__FUNCTION__);  \
        Serial.print("(): ");        \
        Serial.print(__LINE__);      \
        Serial.print(" ] ");         \
        Serial.println(__VA_ARGS__); \
    }
#else
#define DBG(...)
#endif

// DEVICE ID
#define PAJ7620_IIC_ADDR 0x73
#define PAJ7620_PARTID 0x7620

// REGISTER BANK SELECT
#define PAJ7620_REGITER_BANK_SEL (0xEF)

// REGISTER BANK 0
#define PAJ7620_ADDR_PART_ID_LOW 0x00
#define PAJ7620_ADDR_PART_ID_HIGH 0x01
#define PAJ7620_ADDR_VERSION_ID 0x01
#define PAJ7620_ADDR_SUSPEND_CMD 0x03
#define PAJ7620_ADDR_GES_PS_DET_MASK_0 0x41
#define PAJ7620_ADDR_GES_PS_DET_MASK_1 0x42
#define PAJ7620_ADDR_GES_PS_DET_FLAG_0 0x43
#define PAJ7620_ADDR_GES_PS_DET_FLAG_1 0x44
#define PAJ7620_ADDR_STATE_INDICATOR 0x45
#define PAJ7620_ADDR_PS_HIGH_THRESHOLD 0x69
#define PAJ7620_ADDR_PS_LOW_THRESHOLD 0x6A
#define PAJ7620_ADDR_PS_APPROACH_STATE 0x6B
#define PAJ7620_ADDR_PS_RAW_DATA 0x6C

// REGISTER BANK 1
#define PAJ7620_ADDR_PS_GAIN 0x44
#define PAJ7620_ADDR_IDLE_S1_STEP_0 0x67
#define PAJ7620_ADDR_IDLE_S1_STEP_1 0x68
#define PAJ7620_ADDR_IDLE_S2_STEP_0 0x69
#define PAJ7620_ADDR_IDLE_S2_STEP_1 0x6A
#define PAJ7620_ADDR_OP_TO_S1_STEP_0 0x6B
#define PAJ7620_ADDR_OP_TO_S1_STEP_1 0x6C
#define PAJ7620_ADDR_OP_TO_S2_STEP_0 0x6D
#define PAJ7620_ADDR_OP_TO_S2_STEP_1 0x6E
#define PAJ7620_ADDR_OPERATION_ENABLE 0x72

#define PAJ7620_BANK0 0
#define PAJ7620_BANK1 1

// PAJ7620_ADDR_SUSPEND_CMD
#define PAJ7620_I2C_WAKEUP 0x01
#define PAJ7620_I2C_SUSPEND 0x00

// PAJ7620_ADDR_OPERATION_ENABLE
#define PAJ7620_ENABLE 0x01
#define PAJ7620_DISABLE 0x00

#define GES_REACTION_TIME 50  ///< You can adjust the reaction time according to the actual circumstance.
#define GES_ENTRY_TIME 2000  ///< When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s).
#define GES_QUIT_TIME 1000

#define ERR_OK 0           ///< OK
#define ERR_DATA_BUS -1    ///< Error in Data Bus
#define ERR_IC_VERSION -2  ///< IC version mismatch

/**
 * @class MatrixGesture
 * @brief A class for MATRIX Gesture Seneor.
 *
 * This class allows reading 9 gesture from the MATRIX Gesture Seneor.
 */
class MatrixGesture {
   public:
    uint8_t _ch = 0;
    TwoWire* _pWire;
	
	/**
     * @enum  eGesture_t
     * @brief gesture information
     */
    typedef enum {
        eGestureNone = 0x00,           /**< no gestures detected */
        eGestureRight = 0x01 << 0,     /**< move from left to right */
        eGestureLeft = 0x01 << 1,      /**< move from right to left */
        eGestureUp = 0x01 << 2,        /**< move from down to up */
        eGestureDown = 0x01 << 3,      /**< move form up to down */
        eGestureForward = 0x01 << 4,   /**< starts far, move close to sensor */
        eGestureBackward = 0x01 << 5,  /**< starts near, move far to sensor */
        eGestureClockwise = 0x01 << 6, /**< clockwise */
        eGestureAntiClockwise = 0x01 << 7,      /**< anti-clockwise */
        eGestureWave = 0x01 << 8,               /**< wave quickly */
        eGestureWaveSlowlyDisorder = 0x01 << 9, /**< wave randomly and slowly */
        eGestureWaveSlowlyLeftRight =
            eGestureLeft + eGestureRight, /**< slowly move left and right */
        eGestureWaveSlowlyUpDown =
            eGestureUp + eGestureDown, /**< slowly move up and down */
        eGestureWaveSlowlyForwardBackward =
            eGestureForward +
            eGestureBackward, /**< slowly move forward and backward */
        eGestureAll = 0xff /**< support all gestures, no practical meaning, only
                              suitable for writing abstract program logic. */
    } eGesture_t;
	
    /**
     * @enum  eBank_t
     * @brief Register Bank
     */
    typedef enum {
        eBank0 = 0, /**< some registers are located in Bank0*/
        eBank1 = 1, /**< some registers are located in Bank1*/
    } eBank_t;

    /**
     * @enum  eRateMode_t
     * @brief Gesture Update Rate
     */
    typedef enum {
        eNormalRate = 0, /**< Gesture Update Rate is 120HZ, Gesture speed is
                            60°/s - 600°/s*/
        eGamingRate = 1, /**< Gesture Update Rate is 240HZ,Gesture speed is
                            60°/s - 1200°/s*/
    } eRateMode_t;

    /**
     * @struct  int
     * @brief Gesture Description
     */
    typedef struct {
        int gesture;      /**< Gesture enumeration variable X */
        const char* description; /**< Description about the gesture enumeration
                                    variable X */
    } sGestureDescription_t;

    /**
     * @fn begin
     * @brief init function
     * @return return 0 if initialization succeeds, otherwise return non-zero.
     */
    int begin(void);

    /**
     * @fn setGestureHighRate
     * @brief Set gesture detection mode
     * @param b true Set to fast detection mode (default) can recognize 9 gestures, False: slow mode will have 4 expanded gestures.
     */
    void setGestureHighRate(bool b);

    /**
     * @fn gestureDescription
     * @brief Get the string descritpion corresponding to the gesture number.
     * @param gesture Gesture number inlcuded in the int
     * @return Textual description corresponding to the gesture number:if the
     * gesture input in the gesture table doesn't exist,
     * @n return null string.
     */
    String gestureDescription(int gesture);

    /**
     * @fn getGesture
     * @brief Get gesture
     * @return Return gesture, could be any value except Gesture Code in int
     */
    int getGesture(void);

   private:
    void i2cMUXSelect();

    /**
     * @fn selectBank
     * @brief Switch Bank
     * @param bank  The bank you will switch to, eBank0 or eBank1
     * @return Return 0 if switching successfully, otherwise return non-zero.
     */
    int selectBank(eBank_t bank);

    /**
     * @fn setNormalOrGamingMode
     * @brief Set rate mode of the module, the API is disabled currently.
     * @param mode The mode users can configure, eNormalRate or eGamingRate
     * @return Return 0 if setting is successful, otherwise return non-zero.
     */
    int setNormalOrGamingMode(eRateMode_t mode);

    /**
     * @fn writeReg
     * @brief Write register function
     * @param reg  register address 8bits
     * @param pBuf Storage cache of the data to be written into
     * @param size Length of the data to be written into
     */
    void writeReg(uint8_t reg, const void* pBuf, size_t size);

    /**
     * @fn readReg
     * @brief Read register function
     * @param reg  register address 8bits
     * @param pBuf Storage cache of the data to be read
     * @param size Length of the data to be read
     * @return Return the actually read length, fails to read if return 0.
     */
    uint8_t readReg(uint8_t reg, void* pBuf, size_t size);

    /*! init configuration table */
    static const uint8_t /*PROGMEM*/ initRegisterArray[219][2];
    /*! Table of gesture number corresponding to string description */
    static const sGestureDescription_t /*PROGMEM*/ gestureDescriptionsTable[14];
    /*! IIC address of PAJ7620U2, cannot be revised.*/
    const uint8_t _deviceAddr = PAJ7620_IIC_ADDR;
    /*! Whether it is set to fast detection mode, refer to setGestureHighRate
     * for details*/
    bool _gestureHighRate = true;
    /*! Current gesture*/
    eGesture_t _gesture;

};

#endif