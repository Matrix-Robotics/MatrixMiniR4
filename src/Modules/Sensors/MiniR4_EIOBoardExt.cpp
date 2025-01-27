/**
 * @file MiniR4_EIOBoardExt.cpp
 * @brief Handling Matrix Erobot I/O Board functions.
 * @author MATRIX Robotics
 */
#include "MiniR4_EIOBoardExt.h"

void MatrixEIOBoard::digitalOut(uint8_t pins){
	i2cMUXSelect();
	i2cWriteData(0x01, pins, 0x00, 0x00); // act, range, pwm1, pwm2
}

// void MatrixEIOBoard::pwmOut(uint8_t pwm1, uint8_t pwm2){
	// i2cMUXSelect();
	// i2cWriteData(0x00, 255, pwm1, pwm2); // act, range, pwm1, pwm2
// }

void MatrixEIOBoard::i2cMUXSelect(){
    if (_ch < 0) return;   // no MUX
    _pWire->beginTransmission(ADDR_PCA954X);
    _pWire->write((1 << _ch));
    _pWire->endTransmission(1);
    delayMicroseconds(300);
}

void MatrixEIOBoard::i2cWriteData(uint8_t action, uint8_t range, uint8_t pwm1, uint8_t pwm2){

	_pWire->beginTransmission(MatrixEIOBoard_ADDR);

	_pWire->write(action);
	_pWire->write(range);
	_pWire->write(pwm1);
	_pWire->write(pwm2);

	_pWire->endTransmission(1);
}
