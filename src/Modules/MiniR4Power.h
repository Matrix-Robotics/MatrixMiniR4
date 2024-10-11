/**
 * @file MiniR4Power.h
 * @brief Handling Battery Managment functions.
 * @author MATRIX Robotics
 */
#ifndef MINIR4POWER_H
#define MINIR4POWER_H

#include "MMLower.h"

/**
 * @brief Class for managing battery power settings and retrieval.
 *
 * This class provides methods to set battery parameters based on the number of cells
 * and to retrieve the current battery voltage and percentage.
 */
class MiniR4Power
{
public:
    MiniR4Power() {}

    /**
     * @brief Sets the battery parameters based on the number of cells.
     *
     * @param cell The number of 18650 battery cells (2-6).
     * @return true if the parameters were set successfully, false otherwise.
     */
    bool setBattCell(uint8_t cell)
    {
        switch (cell) {
        case 2: return (mmL.SetPowerParam(8.4, 6.8, 7.4) == MMLower::RESULT::OK); break;
        case 3: return (mmL.SetPowerParam(12.6, 10.2, 11.1) == MMLower::RESULT::OK); break;
        case 4: return (mmL.SetPowerParam(16.8, 13.6, 14.8) == MMLower::RESULT::OK); break;
        case 5: return (mmL.SetPowerParam(21.0, 17.0, 18.5) == MMLower::RESULT::OK); break;
        case 6: return (mmL.SetPowerParam(25.2, 20.4, 22.2) == MMLower::RESULT::OK); break;
        default: return false; break;
        }
    }

    /**
     * @brief Retrieves the current battery voltage.
     *
     * @return The voltage of the battery, or 0 if retrieval failed.
     */
    float getBattVoltage(void)
    {
        float voltage = 0, perc = 0;
        if (mmL.GetPowerInfo(voltage, perc) == MMLower::RESULT::OK)
            return voltage;
        else
            return 0;
    }

    /**
     * @brief Retrieves the current battery percentage.
     *
     * @return The percentage of battery charge, or 0 if retrieval failed.
     */
    float getBattPercentage(void)
    {
        float voltage = 0, perc = 0;
        if (mmL.GetPowerInfo(voltage, perc) == MMLower::RESULT::OK)
            return perc;
        else
            return 0;
    }

private:
};

#endif   // MINIR4POWER_H
