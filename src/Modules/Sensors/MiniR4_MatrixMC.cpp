/**
 * @file MiniR4_MatrixMC.cpp
 * @brief Library for working with MATRIX Motor Controller Lite (12V).
 *
 * @author Barry, Anthony RE
 * @license MIT
 */
#include "MiniR4_MatrixMC.h"
#include <Arduino_CAN.h>
#include "FspTimer.h"

// On-wire frame length. Historically this was sizeof(uint8_t*) == 4 by
// accident; the controllers expect these 4-byte frames, so it is kept as an
// explicit constant. Changing it changes the on-wire protocol — do not
// touch without hardware testing.
static const uint8_t  CAN_FRAME_DLC = 4;

// Controllers drop frames that arrive <10 ms after the previous frame.
static const uint32_t FRAME_SPACING_MS = 10;

// Heartbeat period per controller. Firmware timeout is 100 ms per
// docs/firmware-heartbeat-requirements.md, so 20 ms tolerates 4 lost
// heartbeats before the controllers stop their motors.
static const uint32_t HEARTBEAT_PERIOD_MS = 20;

static FspTimer _hbTimer;

// hbTimerCallback 透過 p_args->p_context 拿到 MatrixMC_Class 指標
// 不依賴任何全域物件名稱
static void hbTimerCallback(timer_callback_args_t* p_args) {
  if (p_args && p_args->p_context) {
    static_cast<MatrixMC_Class*>(const_cast<void*>(p_args->p_context))->hbTick();
  }
}

/* =========================
   MatrixMC_Motor
   ========================= */

MatrixMC_Motor::MatrixMC_Motor(uint8_t controllerAddr, uint8_t motorNum, MatrixMC_Class* parent)
  : _controllerAddr(controllerAddr), _motorNum(motorNum), _parent(parent) {
}

void MatrixMC_Motor::begin(void) {
  _initialized = true;
}

void MatrixMC_Motor::setReverse(bool reverse) {
  _reverse = reverse;
  if (!_initialized) return;
  if (!_reverse) {
    uint8_t Motor_Reverse[] = {0x01, 0x60, _motorNum, 0x01, 0x00, 0x00, 0x00, 0x00};
    sendCANMessage(Motor_Reverse);
  } else {
    uint8_t Motor_Reverse[] = {0x01, 0x60, _motorNum, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendCANMessage(Motor_Reverse);
  }
}

void MatrixMC_Motor::setPPR(int ppr) {
  if (ppr <= 0) return;  // 0 would divide by zero in getDegrees()
  _ppr = ppr;
}

void MatrixMC_Motor::resetCounter(void) {
  if (!_initialized) return;
  uint8_t ENC_RST[] = {0x04, 0x60, _motorNum, 0x01, 0x00, 0x00, 0x00, 0x00};

  // A single reset frame can be lost (e.g. colliding with the heartbeat,
  // or right after the Arduino restarts while the controller keeps its
  // old count), so send it and read the counter back until the
  // controller confirms the count is really near zero.
  for (int attempt = 0; attempt < 5; attempt++) {
    sendCANMessage(ENC_RST);
    delay(20);
    // Discard responses that were queued before the reset.
    while (CAN.available()) {
      (void)CAN.read();
    }
    // Only a confirmed fresh reading near zero counts as success: a
    // timed-out read leaves _counter at its previous value (0 right
    // after an Arduino restart), which would fake a pass.
    if (getCounter() && _counter > -5 && _counter < 5) {
      return;
    }
  }
  #ifdef MXMC_DBG
  Serial.println("Encoder reset not confirmed");
  #endif
  _counter = 0;
}

// Returns true when a fresh response was received; on timeout _counter
// keeps its previous value and false is returned.
bool MatrixMC_Motor::getCounter(void) {
  if (!_initialized) return false;
  uint8_t ENC_CNT[] = {0x03, 0x60, _motorNum, 0x00, 0x00, 0x00, 0x00, 0x00};
  sendCANMessage(ENC_CNT);

  // NOTE: CAN.available()/CAN.read() run here with interrupts enabled while
  // the heartbeat ISR may call CAN.write() concurrently. Whether the
  // Arduino_CAN driver's TX and RX paths are independent has not been
  // verified — suspected safe, unconfirmed.
  unsigned long start = millis();
  while (millis() - start < 20) {
    while (CAN.available()) {
      CanMsg const rx_msg = CAN.read();
      if (rx_msg.data[0] == 0x06 && rx_msg.data[1] == 0x60 && rx_msg.data[2] == _motorNum) {
        // The payload does not identify the sender, so MC1.M1 and MC2.M1
        // responses look identical — filter on the CAN id. The documented
        // convention is id == controller address (0x11–0x14); if the
        // firmware answers under a different id scheme, single-controller
        // setups still work (no ambiguity possible) and the observed id is
        // printed once so the filter can be corrected.
        if (rx_msg.id != _controllerAddr) {
          if (!_parent->singleControllerEnabled()) continue;
          static bool idWarned = false;
          if (!idWarned) {
            idWarned = true;
            #ifdef MXMC_DBG
            Serial.print("Encoder resp CAN id 0x");
            Serial.print(rx_msg.id, HEX);
            Serial.print(" (expected 0x");
            Serial.print(_controllerAddr, HEX);
            Serial.println(")");
            #endif
          }
        }
        uint8_t byteArray[4];
        for (int i = 3; i < 7; i++) {
          byteArray[i - 3] = rx_msg.data[i];
        }
        _counter = bytesToInt32Memcpy(byteArray);
        return true;
      }
    }
  }
  return false;
}

int MatrixMC_Motor::getDegrees(void) {
  if (!_initialized) {
    _lastReadFresh = false;
    return 0;
  }
  _lastReadFresh = getCounter();
  // 64-bit intermediate: _counter * 360 overflows int32 past ~5.9M counts
  // (~15k revolutions at 400 PPR), reachable in long-running applications.
  int degrees = (int)(((int64_t)_counter * 360) / _ppr);
  return _reverse ? -degrees : degrees;
}

bool MatrixMC_Motor::lastReadFresh(void) const {
  return _lastReadFresh;
}

void MatrixMC_Motor::setPower(int16_t power) {
  if (!_initialized) return;
  // Protocol range is -100…100; firmware behavior outside it is unknown.
  if (power >  100) power =  100;
  if (power < -100) power = -100;
  uint8_t Motor_Power[] = {0x02, 0x60, _motorNum, (uint8_t)power, 0x00, 0x00, 0x00, 0x00};
  sendCANMessage(Motor_Power);
}

void MatrixMC_Motor::setBrake(bool brake) {
  if (!_initialized) return;
  if (!brake) {
    uint8_t Motor_Brake[] = {0x05, 0x60, _motorNum, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendCANMessage(Motor_Brake);
  } else {
    uint8_t Motor_Brake[] = {0x05, 0x60, _motorNum, 0x01, 0x00, 0x00, 0x00, 0x00};
    sendCANMessage(Motor_Brake);
  }
}

int32_t MatrixMC_Motor::bytesToInt32Memcpy(uint8_t byteArray[4]) {
  // Assemble unsigned first: a signed left shift into the sign bit
  // (byteArray[0] >= 0x80, i.e. negative counts) is UB before C++20.
  uint32_t u = ((uint32_t)byteArray[0] << 24) | ((uint32_t)byteArray[1] << 16)
             | ((uint32_t)byteArray[2] <<  8) |  (uint32_t)byteArray[3];
  return (int32_t)u;
}

void MatrixMC_Motor::sendCANMessage(uint8_t* data) {
  _parent->sendFrame(_controllerAddr, data);
}

/* =========================
   MatrixMC_Servo
   ========================= */

MatrixMC_Servo::MatrixMC_Servo(uint8_t controllerAddr, uint8_t servoNum, MatrixMC_Class* parent)
  : _controllerAddr(controllerAddr), _servoNum(servoNum), _parent(parent) {
}

void MatrixMC_Servo::begin(void) {
  _initialized = true;
}

void MatrixMC_Servo::setReverse(bool reverse) {
  _reverse = reverse;
  if (!_initialized) return;
  if (!_reverse) {
    uint8_t Servo_reverse[] = {0x01, 0x70, _servoNum, 0x01, 0x00, 0x00, 0x00, 0x00};
    sendCANMessage(Servo_reverse);
  } else {
    uint8_t Servo_reverse[] = {0x01, 0x70, _servoNum, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendCANMessage(Servo_reverse);
  }
}

void MatrixMC_Servo::setAngle(uint8_t angle) {
  if (!_initialized) return;
  // Servo range is 0–180; firmware behavior above it is unknown.
  if (angle > 180) angle = 180;
  uint8_t Servo_Angle[] = {0x02, 0x70, _servoNum, angle, 0x00, 0x00, 0x00, 0x00};
  sendCANMessage(Servo_Angle);
}

void MatrixMC_Servo::sendCANMessage(uint8_t* data) {
  _parent->sendFrame(_controllerAddr, data);
}

/* =========================
   MatrixMC_Controller
   ========================= */

MatrixMC_Controller::MatrixMC_Controller(uint8_t address, MatrixMC_Class* parent)
  : _address(address), _parent(parent),
    M1(address, 1, parent), M2(address, 2, parent),
    M3(address, 3, parent), M4(address, 4, parent),
    RC1(address, 1, parent), RC2(address, 2, parent) {
}

void MatrixMC_Controller::begin(void) {
  _initialized = true;
  M1.begin(); M2.begin(); M3.begin(); M4.begin();
  RC1.begin(); RC2.begin();
}

void MatrixMC_Controller::emergency(void) {
  M1.setBrake(true); M2.setBrake(true);
  M3.setBrake(true); M4.setBrake(true);
}

void MatrixMC_Controller::setFrequency(uint8_t fre) {
  if (!_initialized) return;
  if (fre > 20) fre = 20;
  if (fre < 1)  fre = 1;
  uint8_t Motor_Frequency[] = {0x07, 0x60, fre, 0x00, 0x00, 0x00, 0x00, 0x00};
  sendCANMessage(Motor_Frequency);
}

void MatrixMC_Controller::sendCANMessage(uint8_t* data) {
  _parent->sendFrame(_address, data);
}

/* =========================
   MatrixMC_Class
   ========================= */

MatrixMC_Class::MatrixMC_Class()
  : MC1(0x11, this), MC2(0x12, this),
    MC3(0x13, this), MC4(0x14, this) {
  _msTick         = 0;
  _hbTxFail       = 0;
  _hbTimerRunning = false;
  _begun          = false;
  for (int i = 0; i < 4; i++) {
    _devEnabled[i] = false;
    // Start "in the past" so the first frame / first heartbeat of each
    // controller is not held back by the spacing / period checks.
    _lastTx[i] = (uint32_t)0 - FRAME_SPACING_MS;
    _hbLast[i] = (uint32_t)0 - HEARTBEAT_PERIOD_MS;
  }
}

void MatrixMC_Class::begin(bool dev1, bool dev2, bool dev3, bool dev4) {
  // Idempotent: a second call would grab another GPT channel and re-init
  // the running heartbeat timer.
  if (_begun) return;
  _begun = true;

  if (!CAN.begin(CanBitRate::BR_500k)) {
    #ifdef MXMC_DBG
    Serial.println("CAN initialization failed!");
    #endif
    while (1);
  } else {
    #ifdef MXMC_DBG
    Serial.println("CAN initialization succeeded!");
    #endif
  }

  _devEnabled[0] = dev1;
  _devEnabled[1] = dev2;
  _devEnabled[2] = dev3;
  _devEnabled[3] = dev4;

  // Heartbeats start flowing before the (slow) init sequence below, so the
  // controllers see a live link while it runs.
  if (!startHeartbeatTimer()) {
    #ifdef MXMC_DBG
    Serial.println("Heartbeat timer initialization failed!");
    #endif
    while (1);
  }
  _hbTimerRunning = true;

  MatrixMC_Controller* const mcs[4] = { &MC1, &MC2, &MC3, &MC4 };
  for (int i = 0; i < 4; i++) {
    if (!_devEnabled[i]) continue;
    mcs[i]->begin();
  }

  stopAllMotors();
}

void MatrixMC_Class::loop(void) {
  // Heartbeats are sent from the dedicated timer ISR (hbTick). Kept as a
  // no-op so existing sketches that call it keep compiling.
}

bool MatrixMC_Class::startHeartbeatTimer(void) {
  uint8_t timerType = GPT_TIMER;
  int8_t channel = FspTimer::get_available_timer(timerType);
  if (channel < 0) {
    channel = FspTimer::get_available_timer(timerType, true);
    if (channel >= 0) {
      FspTimer::force_use_of_pwm_reserved_timer();
    }
  }
  if (channel < 0) return false;

  // 傳入 this 作為 context，callback 透過 p_args->p_context 取回指標
  // 不依賴任何全域物件名稱
  if (!_hbTimer.begin(TIMER_MODE_PERIODIC, timerType, (uint8_t)channel,
                      1000.0f, 0.0f, hbTimerCallback, this)) return false;
  if (!_hbTimer.setup_overflow_irq()) return false;
  if (!_hbTimer.open())               return false;
  if (!_hbTimer.start())              return false;
  return true;
}

// Runs inside the timer ISR: must stay delay-free and must not touch
// Serial. Sends at most one frame per 1 ms tick, and defers a due
// heartbeat until >=10 ms after the last frame to that controller so it is
// never dropped by the controllers' back-to-back rule.
void MatrixMC_Class::hbTick(void) {
  _msTick = _msTick + 1;
  uint32_t now = _msTick;
  static const uint8_t HBT[] = {0x01, 0x90, 0x00, 0x00};

  for (int i = 0; i < 4; i++) {
    if (!_devEnabled[i]) continue;
    if ((uint32_t)(now - _hbLast[i]) < HEARTBEAT_PERIOD_MS) continue;
    if ((uint32_t)(now - _lastTx[i]) < FRAME_SPACING_MS)    continue;

    CanMsg msg(0x11 + i, CAN_FRAME_DLC, HBT);
    if (CAN.write(msg)) {
      _lastTx[i] = now;
    } else {
      _hbTxFail = _hbTxFail + 1;
    }
    // On failure the beat is skipped (not retried every tick) so one bad
    // controller cannot starve the others' heartbeats.
    _hbLast[i] = now;
    break;
  }
}

uint32_t MatrixMC_Class::heartbeatTxFailCount(void) const {
  return _hbTxFail;
}

bool MatrixMC_Class::singleControllerEnabled(void) const {
  int n = 0;
  for (int i = 0; i < 4; i++) {
    if (_devEnabled[i]) n++;
  }
  return n == 1;
}

void MatrixMC_Class::sendFrame(uint8_t ctrlAddr, const uint8_t* data) {
  int idx = (ctrlAddr >= 0x11 && ctrlAddr <= 0x14) ? (ctrlAddr - 0x11) : -1;

  // The spacing check and the write must be atomic against the heartbeat
  // ISR: checking first and writing after would leave a window where a
  // heartbeat slips in between, putting this frame <10 ms behind it. The
  // millis() bound only applies before the timer runs (there _msTick never
  // advances, so the spacing check would spin forever); once the timer is
  // running it must not fire — a heartbeat sent mid-wait pushes the spacing
  // deadline out, and force-sending at the millis() bound would put this
  // frame <10 ms behind that heartbeat, so the controller drops it. The
  // wait stays bounded (~20 ms worst case: at most one heartbeat per
  // 20 ms period can defer us).
  uint32_t startWait = millis();
  bool ok = false;
  for (;;) {
    noInterrupts();
    bool spacingOk = (idx < 0)
                  || ((uint32_t)(_msTick - _lastTx[idx]) >= FRAME_SPACING_MS)
                  || (!_hbTimerRunning
                      && (uint32_t)(millis() - startWait) >= FRAME_SPACING_MS);
    if (spacingOk) {
      CanMsg msg(ctrlAddr, CAN_FRAME_DLC, data);
      ok = CAN.write(msg);
      if (idx >= 0) {
        _lastTx[idx] = _msTick;
      }
      interrupts();
      break;
    }
    interrupts();
  }

  if (!ok) {
    #ifdef MXMC_DBG
    Serial.println("Failed to send CAN message");
    #endif
  }
  delay(10);
}

void MatrixMC_Class::stopAllMotors(void) {
  MatrixMC_Controller* const mcs[4] = { &MC1, &MC2, &MC3, &MC4 };
  for (int i = 0; i < 4; i++) {
    if (!_devEnabled[i]) continue;
    mcs[i]->M1.setPower(0);
    mcs[i]->M2.setPower(0);
    mcs[i]->M3.setPower(0);
    mcs[i]->M4.setPower(0);
  }
}