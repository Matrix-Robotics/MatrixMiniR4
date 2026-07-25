/**
 * @file MiniR4_MatrixMC.h
 * @brief Library for working with MATRIX Motor Controller Lite (12V).
 *
 * @author Barry, Anthony RE
 * @license MIT
 */
#ifndef MINIR4_MatrixMC_h
#define MINIR4_MatrixMC_h

#define MXMC_DBG

#include <Arduino.h>

// Forward declarations
class MatrixMC_Class;

/* =========================
   Motor Class
   ========================= */
/**
 * @brief Class for controlling a single DC motor via CAN bus through a MatrixMC controller.
 */
class MatrixMC_Motor {
public:
  /**
   * @brief Constructor. Binds this motor to its controller address, motor number, and parent instance.
   * @param controllerAddr CAN address of the parent controller
   * @param motorNum       Motor index on the controller (1–4)
   * @param parent         Pointer to the owning MatrixMC_Class instance
   */
  MatrixMC_Motor(uint8_t controllerAddr, uint8_t motorNum, MatrixMC_Class* parent);

  /**
   * @brief Initialize the motor and send the initial configuration to the controller.
   */
  void begin(void);

  /**
   * @brief Set whether the motor direction should be reversed.
   * @param reverse true to invert direction; false (default) for normal direction
   */
  void setReverse(bool reverse = false);

  /**
   * @brief Set the encoder pulses-per-revolution (PPR).
   * @param ppr Pulses per revolution; default is 400
   */
  void setPPR(int ppr = 400);

  /**
   * @brief Reset the encoder counter to zero.
   */
  void resetCounter(void);

  /**
   * @brief Get the current motor rotation in degrees.
   * @return Calculated degree value; returns the last cached value if the read timed out
   */
  int getDegrees(void);

  /**
   * @brief Check whether the last getDegrees() call received a fresh encoder response.
   * @return true if the data is newly read; false if the read timed out and a stale value was returned
   */
  bool lastReadFresh(void) const;

  /**
   * @brief Set the motor output power.
   * @param power Output power in the range -100 to 100 (negative values reverse direction)
   */
  void setPower(int16_t power);

  /**
   * @brief Set the motor brake state.
   * @param brake true to engage braking; false to release
   */
  void setBrake(bool brake);

private:
  uint8_t         _controllerAddr;      ///< CAN address of the parent controller
  uint8_t         _motorNum;            ///< Motor index on the controller
  MatrixMC_Class* _parent;              ///< Pointer to the owning MatrixMC_Class instance
  bool    _initialized   = false;       ///< Whether begin() has been called
  bool    _reverse       = false;       ///< Whether direction is inverted
  int     _ppr           = 400;         ///< Encoder pulses per revolution
  int32_t _counter       = 0;           ///< Last received encoder count (cached)
  bool    _lastReadFresh = false;       ///< Whether the last encoder read returned fresh data

  /**
   * @brief Send an 8-byte control frame over the CAN bus.
   * @param data Pointer to an 8-byte data array
   */
  void sendCANMessage(uint8_t* data);

  /**
   * @brief Convert a 4-byte array to int32_t using memcpy.
   * @param byteArray Source 4-byte array
   * @return Converted 32-bit signed integer
   */
  int32_t bytesToInt32Memcpy(uint8_t byteArray[4]);

  /**
   * @brief Request and update the encoder count from the controller into _counter.
   * @return true if fresh data was received; false if the request timed out
   */
  bool getCounter(void);
};

/* =========================
   Servo Class
   ========================= */
/**
 * @brief Class for controlling a single servo motor via CAN bus through a MatrixMC controller.
 */
class MatrixMC_Servo {
public:
  /**
   * @brief Constructor. Binds this servo to its controller address, servo number, and parent instance.
   * @param controllerAddr CAN address of the parent controller
   * @param servoNum       Servo index on the controller (1–2)
   * @param parent         Pointer to the owning MatrixMC_Class instance
   */
  MatrixMC_Servo(uint8_t controllerAddr, uint8_t servoNum, MatrixMC_Class* parent);

  /**
   * @brief Initialize the servo and send the initial configuration to the controller.
   */
  void begin(void);

  /**
   * @brief Set whether the servo direction should be reversed.
   * @param reverse true to invert direction; false (default) for normal direction
   */
  void setReverse(bool reverse = false);

  /**
   * @brief Command the servo to move to the specified angle.
   * @param angle Target angle in degrees (0–180)
   */
  void setAngle(uint8_t angle);

private:
  uint8_t         _controllerAddr; ///< CAN address of the parent controller
  uint8_t         _servoNum;       ///< Servo index on the controller
  MatrixMC_Class* _parent;         ///< Pointer to the owning MatrixMC_Class instance
  bool    _initialized = false;    ///< Whether begin() has been called
  bool    _reverse     = false;    ///< Whether direction is inverted

  /**
   * @brief Send an 8-byte control frame over the CAN bus.
   * @param data Pointer to an 8-byte data array
   */
  void sendCANMessage(uint8_t* data);
};

/* =========================
   Controller Class
   ========================= */
/**
 * @brief Class representing a single MatrixMC motor controller, managing
 *        four DC motors and two servo motors.
 */
class MatrixMC_Controller {
public:
  /**
   * @brief Constructor. Binds the controller to its CAN address and parent instance.
   * @param address CAN address of this controller
   * @param parent  Pointer to the owning MatrixMC_Class instance
   */
  MatrixMC_Controller(uint8_t address, MatrixMC_Class* parent);

  /**
   * @brief Initialize this controller and all of its motor and servo instances.
   */
  void begin(void);

  /**
   * @brief Send an emergency stop command to immediately cut all motor outputs.
   */
  void emergency(void);

  /**
   * @brief Set the PWM frequency for the motor outputs.
   * @param fre Frequency setting value (maps to firmware-defined frequency steps)
   */
  void setFrequency(uint8_t fre);

  MatrixMC_Motor M1; ///< DC motor 1
  MatrixMC_Motor M2; ///< DC motor 2
  MatrixMC_Motor M3; ///< DC motor 3
  MatrixMC_Motor M4; ///< DC motor 4

  MatrixMC_Servo RC1; ///< Servo motor 1
  MatrixMC_Servo RC2; ///< Servo motor 2

private:
  uint8_t         _address;        ///< CAN address of this controller
  MatrixMC_Class* _parent;         ///< Pointer to the owning MatrixMC_Class instance
  bool    _initialized = false;    ///< Whether begin() has been called

  /**
   * @brief Send an 8-byte control frame over the CAN bus.
   * @param data Pointer to an 8-byte data array
   */
  void sendCANMessage(uint8_t* data);
};

/* =========================
   Main Library Class
   ========================= */
/**
 * @brief Top-level MatrixMC library class. Manages up to four controllers,
 *        schedules CAN heartbeat frames via a timer ISR, and provides the
 *        single transmit path for all main-thread CAN frames.
 */
class MatrixMC_Class {
public:
  /**
   * @brief Constructor. Initializes the four controller objects.
   */
  MatrixMC_Class();

  /**
   * @brief Initialize the library, bring up the CAN bus, and start sending
   *        heartbeat frames to all enabled controllers.
   * @param dev1 Enable controller MC1
   * @param dev2 Enable controller MC2
   * @param dev3 Enable controller MC3
   * @param dev4 Enable controller MC4
   */
  void begin(bool dev1, bool dev2, bool dev3, bool dev4);

  /**
   * @brief Backward-compatibility no-op. Heartbeat frames are now sent from
   *        a dedicated timer ISR; calling this in the main loop is no longer required.
   */
  void loop(void);

  /**
   * @brief Internal: single transmit path for all main-thread CAN frames.
   *        Waits out the 10 ms inter-frame spacing and masks interrupts around
   *        the write to prevent the heartbeat ISR from interleaving.
   * @param ctrlAddr CAN address of the target controller
   * @param data     Pointer to an 8-byte data array
   */
  void sendFrame(uint8_t ctrlAddr, const uint8_t* data);

  /**
   * @brief Internal: 1 ms tick called exclusively from the heartbeat timer ISR.
   *        Advances _msTick and triggers heartbeat transmissions at the correct intervals.
   */
  void hbTick(void);

  /**
   * @brief Get the number of heartbeat frames the CAN driver refused to queue (diagnostic).
   * @return Cumulative count of heartbeat transmit failures
   */
  uint32_t heartbeatTxFailCount(void) const;

  /**
   * @brief Check whether exactly one controller was enabled in begin().
   *        When true, the encoder-response filter relaxes the CAN-ID check
   *        because there is no ambiguity about which controller sent the response.
   * @return true if only one controller is enabled
   */
  bool singleControllerEnabled(void) const;

  MatrixMC_Controller MC1; ///< Controller 1
  MatrixMC_Controller MC2; ///< Controller 2
  MatrixMC_Controller MC3; ///< Controller 3
  MatrixMC_Controller MC4; ///< Controller 4

private:
  volatile bool     _devEnabled[4];  ///< Per-controller enable flags (volatile: shared with ISR)
  volatile uint32_t _msTick;         ///< 1 ms timebase maintained by the heartbeat ISR
  volatile uint32_t _lastTx[4];      ///< _msTick timestamp of the last frame sent per controller
  volatile uint32_t _hbLast[4];      ///< _msTick timestamp of the last heartbeat sent per controller
  volatile uint32_t _hbTxFail;       ///< Cumulative heartbeat transmit failure count
  volatile bool     _hbTimerRunning; ///< True once the timer ISR is running; sendFrame then relies on _msTick spacing
  bool              _begun;          ///< Whether begin() has completed

  /**
   * @brief Start the heartbeat timer ISR.
   * @return true if the timer started successfully
   */
  bool startHeartbeatTimer(void);

  /**
   * @brief Send stop commands to all enabled controllers, zeroing every motor output.
   */
  void stopAllMotors(void);
};

#endif