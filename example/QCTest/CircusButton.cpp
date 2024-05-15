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

#include "CircusButton.h"

CircusButton::CircusButton(uint8_t pin, byte uActiveLevel)
    : CircusButton(
          pin, uActiveLevel, _DEFAULT_TIME_KEY_POLLING_uS, _DEFAULT_TIME_KEY_REPEAT_START_uS,
          _DEFAULT_TIME_KEY_REPEAT_WORK_uS)
{}

CircusButton::CircusButton(
    uint8_t pin, byte uActiveLevel, uint32_t timeKeyRepeatStart_us, uint32_t timeKeyRepeatWork_us)
    : CircusButton(
          pin, uActiveLevel, _DEFAULT_TIME_KEY_POLLING_uS, timeKeyRepeatStart_us,
          timeKeyRepeatWork_us)
{}

CircusButton::CircusButton(
    uint8_t pin, byte uActiveLevel, uint32_t timeKeyPolling_us, uint32_t timeKeyRepeatStart_us,
    uint32_t timeKeyRepeatWork_us)
{
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP);
    Init_Polling_Button(uActiveLevel);
    this->timeKeyPolling_us     = timeKeyPolling_us;
    this->timeKeyRepeatStart_us = timeKeyRepeatStart_us;
    this->timeKeyRepeatWork_us  = timeKeyRepeatWork_us;
}

void CircusButton::Init_Polling_Button(byte uActiveLevel)
{
    FillBytes((byte*)&this->pTag, 0x00, sizeof(KEY_POLLING_T));
    pTag.uActiveLevel = uActiveLevel;
}

void CircusButton::FillBytes(byte* pDst, byte fillByte, byte uCount)
{
    byte i;
    for (i = 0; i < uCount; i++) {
        pDst[i] = fillByte;
    }
}

//--------------------------------------------------------
//	Return Code:
//		1. 0x00: No-Key, key is not pressed.
//		2. 0x01: Front-Edge Trigger.
//		3. 0x02: Repeat Trigger.
//		4. 0x03: Key still Pressed.
//		5. 0x04: Rear-Edge Trigger.
//--------------------------------------------------------
void CircusButton::Polling_Button_Repeat()
{
    // Check if it is time to do Polling-Key
    uint32_t dwTime = micros();
    if ((dwTime - pTag.dwTimeSlot_Polling) < this->timeKeyPolling_us) {
        this->btn_state = BTN_STATE::NOKEY;
        return;   // No-Key, time is not up
    }

    // Process Polling Key
    pTag.dwTimeSlot_Polling = dwTime;

    byte uPinLevel = (pTag.bFlag ^ pTag.uActiveLevel) & 0x01;
    if (uPinLevel > 0) {   // Key is not pressed
        pTag.bPressKey = false;

        if (pTag.bEnableRepeat) {
            pTag.bEnableRepeat = false;
            this->btn_state    = BTN_STATE::R_EDGE;
            return;   // Rear-Edge Trigger.
        } else {
            this->btn_state = BTN_STATE::NOKEY;
            return;   // No-Key, key is not pressed.
        }
    } else {   // Key was pressed
        pTag.bPressKey = true;
        if (!pTag.bEnableRepeat) {
            pTag.dwTimeSlot_Repeat = dwTime;
            pTag.dwOrgtTime        = dwTime;
            pTag.dwRepeatTime      = this->timeKeyRepeatStart_us;
            pTag.bEnableRepeat     = true;
            pTag.uRepeatCount      = 0;
            this->btn_state        = BTN_STATE::F_EDGE;
            return;   // Front-Edge Trigger.
        } else {
            if ((dwTime - pTag.dwTimeSlot_Repeat) > pTag.dwRepeatTime) {
                pTag.dwTimeSlot_Repeat = dwTime;
                pTag.dwRepeatTime      = this->timeKeyRepeatWork_us;
                pTag.uRepeatCount++;
                this->btn_state = BTN_STATE::REPEAT;
                return;   // Repeat Trigger.
            } else {
                this->btn_state = BTN_STATE::PRESSED;
                return;   // Key still Pressed.
            }
        }
    }
}

CircusButton::~CircusButton() {}

void CircusButton::SetTKeyPolling(uint32_t timeKeyPolling_us)
{
    this->timeKeyPolling_us = timeKeyPolling_us;
}

void CircusButton::SetTKeyRepeatStart(uint32_t timeKeyRepeatStart_us)
{
    this->timeKeyRepeatStart_us = timeKeyRepeatStart_us;
}

void CircusButton::SetTKeyRepeatWork(uint32_t timeKeyRepeatWork_us)
{
    this->timeKeyRepeatWork_us = timeKeyRepeatWork_us;
}

BTN_STATE CircusButton::readState()
{
    return this->btn_state;
}

// 取得此按鈕當前邏輯狀態
bool CircusButton::readRawLevel()
{
    return digitalRead(this->pin);
}

// Polling
void CircusButton::loop()
{
    this->pTag.bFlag = (bool)digitalRead(this->pin);
    this->Polling_Button_Repeat();
}

void CircusButton::loopEx(bool bFlag)
{
    this->pTag.bFlag = bFlag;
    this->Polling_Button_Repeat();
}