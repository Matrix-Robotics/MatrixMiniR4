#include "MiniR4MxCtrlExt.h"

void MatrixController::begin(){//motor reset / servo reset/set serve speed to 0 (fastest)
	i2cMUXSelect();

	i2cWriteData(MC_REG_TIMEOUT, 0); // disable timeout

	i2cWriteData(MC_REG_MOTOR_MODE, 0x05); // reset motor
	i2cWriteData(MC_REG_MOTOR_MODE+10, 0x05);
	i2cWriteData(MC_REG_MOTOR_MODE+20, 0x05);
	i2cWriteData(MC_REG_MOTOR_MODE+30, 0x05);

	i2cWriteData(MC_REG_SERVO_ENABLE, 0x0F); // enable servo

	i2cWriteData(MC_REG_SERVO_SPEED, 0x0); //set serve speed to fastest
	i2cWriteData(MC_REG_SERVO_SPEED+2, 0x0);
	i2cWriteData(MC_REG_SERVO_SPEED+4, 0x0);
	i2cWriteData(MC_REG_SERVO_SPEED+6, 0x0);
}

void MatrixController::motorSet(uint8_t motor_ch, uint8_t speed){
	i2cMUXSelect();

	motorMode(motor_ch, MODE_BRAKE);
	uint8_t addr = MC_REG_MOTOR_SPEED + (motor_ch-1) * MC_REG_MOTOR_OFFSET;

	i2cWriteData(addr, speed);
	i2cWriteData(addr, speed);
}

void MatrixController::motorRound(uint8_t motor_ch, float round){
	i2cMUXSelect();
	motorSet(motor_ch, 0);

	motorMode(motor_ch, MODE_POSITION);

	uint8_t addr = MC_REG_MOTOR_TARGET + (motor_ch-1) * MC_REG_MOTOR_OFFSET;
	uint32_t target = motorPosition(motor_ch) + round * 756;
	uint32_t data;
	int shift;

	i2cWriteData(addr+4, 100);

	for(int i=0; i<4; i++){
		shift = 24 - (i*8);
		data = target >> shift;
		i2cWriteData(addr+i, data);
		target = target - (data << shift);
	}
	
}

void MatrixController::motorInvert(uint8_t motor_ch, bool invert){
	if(invert){
		switch (motor_ch)
		{
			case 1:
				_invert1 = 0x10;
				break;
			case 2:
				_invert2 = 0x10;
				break;
			case 3:
				_invert3 = 0x10;
				break;
			case 4:
				_invert4 = 0x10;
				break;
			default:
				break;
		}
	}
	else{
		switch (motor_ch)
		{
			case 1:
				_invert1 = 0;
				break;
			case 2:
				_invert2 = 0;
				break;
			case 3:
				_invert3 = 0;
				break;
			case 4:
				_invert4 = 0;
				break;
			default:
				break;
		}
	}
}

void MatrixController::servoSet(uint8_t servo_ch, uint8_t angle){
	i2cMUXSelect();

	if(angle > 180){
		angle = 250;
	}
	else{
		angle = round(float(angle)*1.388);
	}
	uint8_t addr = MC_REG_SERVO_TARGET + (servo_ch-1) * MC_REG_SERVO_OFFSET;

	i2cWriteData(addr, angle);
	i2cWriteData(addr, angle);
}

float MatrixController::getBattery(){
	return (i2cReadData(MC_REG_BATTERY, 1) * 0.04);
}

void MatrixController::motorMode(uint8_t motor_ch, uint8_t mode){
	if(mode){
		switch (motor_ch)
		{
			case 1:
				i2cWriteData(MC_REG_MOTOR_MODE, _invert1 + 0x01);
				break;
			case 2:
				i2cWriteData(MC_REG_MOTOR_MODE + 10, _invert2 + 0x01);
				break;
			case 3:
				i2cWriteData(MC_REG_MOTOR_MODE + 20, _invert3 + 0x01);
				break;
			case 4:
				i2cWriteData(MC_REG_MOTOR_MODE + 30, _invert4 + 0x01);
				break;
			default:
				break;
		}
	}
	else{
		switch (motor_ch)
		{
			case 1:
				i2cWriteData(MC_REG_MOTOR_MODE, _invert1 + 0x03);
				break;
			case 2:
				i2cWriteData(MC_REG_MOTOR_MODE + 10, _invert2 + 0x03);
				break;
			case 3:
				i2cWriteData(MC_REG_MOTOR_MODE + 20, _invert3 + 0x03);
				break;
			case 4:
				i2cWriteData(MC_REG_MOTOR_MODE + 30, _invert4 + 0x03);
				break;
			default:
				break;
		}
	}
}

uint32_t MatrixController::motorPosition(uint8_t motor_ch){
	uint8_t addr = MC_REG_MOTOR_POS + (motor_ch-1) * MC_REG_MOTOR_OFFSET;
	return i2cReadData(addr, 4); // 28ppr * 27:1 = 756 / r
}

void MatrixController::i2cMUXSelect(){
    if (_ch < 0) return;   // no MUX
    _pWire->beginTransmission(ADDR_PCA954X);
    _pWire->write((1 << _ch));
    _pWire->endTransmission(1);
    delayMicroseconds(300);
}

void MatrixController::i2cWriteData(uint8_t reg, uint8_t data){

	_pWire->beginTransmission(MatrixCtrl_ADDR);

	_pWire->write(reg);
	_pWire->write(data);

	_pWire->endTransmission(1);
}

uint32_t MatrixController::i2cReadData(int reg, int num){
	
	_pWire->beginTransmission(MatrixCtrl_ADDR);
	_pWire->write(reg);
	_pWire->endTransmission(1);

	delay(1);

	_pWire->requestFrom(MatrixCtrl_ADDR, num);

	delay(1);

	uint32_t dataBuf = 0;

	int i = 0;

	while (_pWire->available() && i<num){
		dataBuf = dataBuf << 8;
		dataBuf += _pWire->read();
		i++;
	}
	return dataBuf;
}
