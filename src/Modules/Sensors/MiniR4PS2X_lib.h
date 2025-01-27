/**
 * @file MiniR4PS2X_lib.h
 * @brief Merge PS2X Lib into Mini R4.
 */

/******************************************************************
*  Super amazing PS2 controller Arduino Library v1.8
*		details and example sketch:
*			http://www.billporter.info/?p=240
*
*    Original code by Shutter on Arduino Forums
*
*    Revamped, made into lib by and supporting continued development:
*              Bill Porter
*              www.billporter.info
*
*	 Contributers:
*		Eric Wetzel (thewetzel@gmail.com)
*		Kurt Eckhardt
*
*  Lib version history
*    0.1 made into library, added analog stick support.
*    0.2 fixed config_gamepad miss-spelling
*        added new functions:
*          NewButtonState();
*          NewButtonState(unsigned int);
*          ButtonPressed(unsigned int);
*          ButtonReleased(unsigned int);
*        removed 'PS' from beginning of ever function
*    1.0 found and fixed bug that wasn't configuring controller
*        added ability to define pins
*        added time checking to reconfigure controller if not polled enough
*        Analog sticks and pressures all through 'ps2x.Analog()' function
*        added:
*          enableRumble();
*          enablePressures();
*    1.1
*        added some debug stuff for end user. Reports if no controller found
*        added auto-increasing sentence delay to see if it helps compatibility.
*    1.2
*        found bad math by Shutter for original clock. Was running at 50kHz, not
the required 500kHz.
*        fixed some of the debug reporting.
*	1.3
*	    Changed clock back to 50kHz. CuriousInventor says it's suppose to be
500kHz, but doesn't seem to work for everybody. *	1.4 *		Removed
redundant functions.
*		Fixed mode check to include two other possible modes the
controller could be in.
*       Added debug code enabled by compiler directives. See below to enable
debug mode. *		Added button definitions for shapes as well as colors.
*	1.41
*		Some simple bug fixes
*		Added Keywords.txt file
*	1.5
*		Added proper Guitar Hero compatibility
*		Fixed issue with DEBUG mode, had to send serial at once instead
of in bits *	1.6
*		Changed config_gamepad() call to include rumble and pressures
options *			This was to fix controllers that will only go
into config mode once
*			Old methods should still work for backwards
compatibility
*    1.7
*		Integrated Kurt's fixes for the interrupts messing with servo
signals *		Reorganized directory so examples show up in Arduino IDE
menu
*    1.8
*		Added Arduino 1.0 compatibility.
*    1.9
*       Kurt - Added detection and recovery from dropping from analog mode, plus
*       integrated Chipkit (pic32mx...) support
*
*
*
*This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or(at your option) any later
version. This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details. <http://www.gnu.org/licenses/>
*
******************************************************************/

// $$$$$$$$$$$$ DEBUG ENABLE SECTION $$$$$$$$$$$$$$$$
// to debug ps2 controller, uncomment these two lines to print out debug to uart
// #define PS2X_DEBUG
// #define PS2X_COM_DEBUG

#ifndef PS2X_lib_h
#define PS2X_lib_h

#if ARDUINO > 22
#    include "Arduino.h"
#else
#    include "WProgram.h"
#endif

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define CTRL_CLK        5
#define CTRL_CLK_HIGH   5
#define CTRL_BYTE_DELAY 18

// These are our button constants
#define PSB_SELECT    0x0001
#define PSB_L3        0x0002
#define PSB_R3        0x0004
#define PSB_START     0x0008
#define PSB_PAD_UP    0x0010
#define PSB_PAD_RIGHT 0x0020
#define PSB_PAD_DOWN  0x0040
#define PSB_PAD_LEFT  0x0080
#define PSB_L2        0x0100
#define PSB_R2        0x0200
#define PSB_L1        0x0400
#define PSB_R1        0x0800
#define PSB_GREEN     0x1000
#define PSB_RED       0x2000
#define PSB_BLUE      0x4000
#define PSB_PINK      0x8000
#define PSB_TRIANGLE  0x1000
#define PSB_CIRCLE    0x2000
#define PSB_CROSS     0x4000
#define PSB_SQUARE    0x8000

// Guitar  button constants
#define UP_STRUM    0x0010
#define DOWN_STRUM  0x0040
#define LEFT_STRUM  0x0080
#define RIGHT_STRUM 0x0020
#define STAR_POWER  0x0100
#define GREEN_FRET  0x0200
#define YELLOW_FRET 0x1000
#define RED_FRET    0x2000
#define BLUE_FRET   0x4000
#define ORANGE_FRET 0x8000
#define WHAMMY_BAR  8

// These are stick values
#define PSS_RX 5
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

// These are analog buttons
#define PSAB_PAD_RIGHT 9
#define PSAB_PAD_UP    11
#define PSAB_PAD_DOWN  12
#define PSAB_PAD_LEFT  10
#define PSAB_L2        19
#define PSAB_R2        20
#define PSAB_L1        17
#define PSAB_R1        18
#define PSAB_GREEN     13
#define PSAB_RED       14
#define PSAB_BLUE      15
#define PSAB_PINK      16
#define PSAB_TRIANGLE  13
#define PSAB_CIRCLE    14
#define PSAB_CROSS     15
#define PSAB_SQUARE    16

#define SET(x, y) (x |= (1 << y))
#define CLR(x, y) (x &= (~(1 << y)))
#define CHK(x, y) (x & (1 << y))
#define TOG(x, y) (x ^= (1 << y))

/**
 * @brief Class to interface with the MJ2 or PS2 controller.
 * 
 * Once you call MiniR4.begin(), PS2X will auto begin.
 * Note: Please Connect 4p wire to D2 (Left: DAT, Right: CMD, V, G), 2p wire to D3 (Left: CSS/SET, Right: CLK)
 *
 * This class provides methods to read button states, configure the controller, and handle analog inputs.
 */
class PS2X
{
public:
    /**
     * @brief Checks if a button is currently pressed.
     *
     * @param button The button constant to check.
     * @return true if the button is being pressed, false otherwise.
     */
    boolean      Button(uint16_t);   // will be TRUE if button is being pressed
    
    /**
     * @brief Reads the button data byte.
     *
     * @return The button data byte.
     */
    unsigned int ButtonDataByte();

    /**
     * @brief Checks for new button state.
     *
     * @return true if there is a new button state, false otherwise.
     */
    boolean      NewButtonState();

    /**
     * @brief Checks for new button state for a specific button.
     *
     * @param button The button constant to check.
     * @return true if the button was just pressed or released, false otherwise.
     */
    boolean NewButtonState(unsigned int);   // will be TRUE if button was JUST pressed OR released

    /**
     * @brief Checks if a button was just pressed.
     *
     * @param button The button constant to check.
     * @return true if the button was just pressed, false otherwise.
     */
    boolean ButtonPressed(unsigned int);    // will be TRUE if button was JUST
                                            // pressed

    /**
     * @brief Checks if a button was just released.
     *
     * @param button The button constant to check.
     * @return true if the button was just released, false otherwise.
     */
    boolean ButtonReleased(unsigned int);   // will be TRUE if button was JUST released

    /**
     * @brief Reads the gamepad state.
     */
    void    read_gamepad();

    /**
     * @brief Reads the gamepad state with options.
     *
     * @param motor1 Motor1.
     * @param motor2 Motor2.
     * @return true if the read was successful.
     */
    boolean read_gamepad(boolean, byte);

    /**
     * @brief Returns the controller type.
     *
     * @return The controller type byte.
     */
    byte    readType();

    /**
     * @brief Configures the gamepad.
     *
     * @param clk Pin for clock.
     * @param cmd Pin for command.
     * @param att Pin for attention.
     * @param dat Pin for data.
     * @return Status byte.
     */
    byte    config_gamepad(uint8_t, uint8_t, uint8_t, uint8_t);

    /**
     * @brief Configures the gamepad with additional options.
     *
     * @param clk Pin for clock.
     * @param cmd Pin for command.
     * @param att Pin for attention.
     * @param dat Pin for data.
     * @param pressures Enable pressure sensitivity. (MJ2 not avaiable)
     * @param rumble Enable rumble functionality. (MJ2 not avaiable)
     * @return Status byte.
     */
    byte    config_gamepad(uint8_t, uint8_t, uint8_t, uint8_t, bool, bool);

    /**
     * @brief Enables rumble functionality on the controller.
     */
    void    enableRumble();

    /**
     * @brief Enables pressure sensitivity on the controller.
     *
     * @return true if pressures are enabled, false otherwise.
     */
    bool    enablePressures();

    /**
     * @brief Reads the analog value from the specified analog stick.
     *
     * @param stick The analog stick identifier.
     * @return The analog value.
     */
    byte    Analog(byte);

    /**
     * @brief Reconfigures the gamepad.
     */
    void    reconfig_gamepad();

private:
    inline void CLK_SET(void);
    inline void CLK_CLR(void);
    inline void CMD_SET(void);
    inline void CMD_CLR(void);
    inline void ATT_SET(void);
    inline void ATT_CLR(void);
    inline bool DAT_CHK(void);

    unsigned char _gamepad_shiftinout(char);
    unsigned char PS2data[21];
    void          sendCommandString(byte string[], byte len);
    unsigned char i;
    unsigned int  last_buttons;
    unsigned int  buttons;

    int _clk_pin;
    int _cmd_pin;
    int _att_pin;
    int _dat_pin;

    unsigned long last_read;
    byte          read_delay;
    byte          controller_type;
    boolean       en_Rumble;
    boolean       en_Pressures;
};

#endif
