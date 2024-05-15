//***************************************************************************************
// PROJECT: CircusButton.cpp
// VERSION: V1.0 (Build:20230428)
// AUTHOR : Zack Huang
//***************************************************************************************
// Version History:
//
// V1.0: First Test Release
//
//
//***************************************************************************************

#include "Arduino.h"
#ifndef _CIRCUS_BUTTON_H
#    define _CIRCUS_BUTTON_H

#    define _DEFAULT_TIME_KEY_POLLING_uS      50000UL
#    define _DEFAULT_TIME_KEY_REPEAT_START_uS 1000000UL
#    define _DEFAULT_TIME_KEY_REPEAT_WORK_uS  300000UL

typedef struct
{
    uint32_t dwTimeSlot_Polling;
    uint32_t dwTimeSlot_Repeat;
    uint32_t dwRepeatTime;
    uint32_t dwOrgtTime;
    bool     bPressKey;
    bool     bEnableRepeat;
    byte     uActiveLevel;
    byte     uRepeatCount;
    byte     uReserved;
    bool     bFlag;
} KEY_POLLING_T;

enum class BTN_STATE
{
    NOKEY,
    F_EDGE,
    REPEAT,
    PRESSED,
    R_EDGE,
};

class CircusButton
{
private:
    uint8_t       pin;
    KEY_POLLING_T pTag;
    BTN_STATE     btn_state;
    uint32_t      timeKeyPolling_us;
    uint32_t      timeKeyRepeatStart_us;
    uint32_t      timeKeyRepeatWork_us;

    void Init_Polling_Button(byte uActiveLevel);
    void FillBytes(byte* pDst, byte fillByte, byte uCount);
    void Polling_Button_Repeat();

public:
    CircusButton(uint8_t pin, byte uActiveLevel);
    CircusButton(
        uint8_t pin, byte uActiveLevel, uint32_t timeKeyRepeatStart_us,
        uint32_t timeKeyRepeatWork_us);
    CircusButton(
        uint8_t pin, byte uActiveLevel, uint32_t timeKeyPolling_us, uint32_t timeKeyRepeatStart_us,
        uint32_t timeKeyRepeatWork_us);
    ~CircusButton();
    void SetTKeyPolling(uint32_t timeKeyPolling_us);
    void SetTKeyRepeatStart(uint32_t timeKeyRepeatStart_us);
    void SetTKeyRepeatWork(uint32_t timeKeyRepeatWork_us);

    BTN_STATE readState();
    // 取得此按鈕當前邏輯狀態
    bool readRawLevel();
    // Polling
    void loop();
    void loopEx(bool bFlag);
};

#endif
