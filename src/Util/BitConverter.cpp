/**
 * @file BitConverter.cpp
 * @brief MiniR4 low level functions.
 * @author MATRIX Robotics
 */
#include "BitConverter.h"

uint32_t BitConverter::ToUInt32(uint8_t* value, int startIdx)
{
    uint32_t data = value[startIdx++];
    data |= value[startIdx++] << 8;
    data |= value[startIdx++] << 16;
    data |= value[startIdx] << 24;
    return data;
}

uint16_t BitConverter::ToUInt16(uint8_t* value, int startIdx)
{
    uint16_t data = value[startIdx++];
    data |= value[startIdx] << 8;
    return data;
}

int32_t BitConverter::ToInt32(uint8_t* value, int startIdx)
{
    int32_t data = value[startIdx++];
    data |= value[startIdx++] << 8;
    data |= value[startIdx++] << 16;
    data |= value[startIdx] << 24;
    return data;
}

int16_t BitConverter::ToInt16(uint8_t* value, int startIdx)
{
    int16_t data = value[startIdx++];
    data |= value[startIdx] << 8;
    return data;
}

void BitConverter::GetBytes(uint8_t* buff, uint32_t value)
{
    buff[0] = (uint8_t)(value & 0xFF);
    buff[1] = (uint8_t)((value >> 8) & 0xFF);
    buff[2] = (uint8_t)((value >> 16) & 0xFF);
    buff[3] = (uint8_t)((value >> 24) & 0xFF);
}

void BitConverter::GetBytes(uint8_t* buff, int32_t value)
{
    GetBytes(buff, static_cast<uint32_t>(value));
}

void BitConverter::GetBytes(uint8_t* buff, uint16_t value)
{
    buff[0] = (uint8_t)(value & 0xFF);
    buff[1] = (uint8_t)((value >> 8) & 0xFF);
}

void BitConverter::GetBytes(uint8_t* buff, int16_t value)
{
    GetBytes(buff, static_cast<uint16_t>(value));
}
