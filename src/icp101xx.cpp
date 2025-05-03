/******************************************************************
 Library for the TDK InvenSense ICP-101xx series of barometric 
 pressure sensors. These sensors communicate over I2C.
 
 Tested with ICP-10100, but likely to also work with ICP-10101,
 ICP-10110 and ICP-10111.
 
 Written by Adrian Studer.
 
 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.
******************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include "icp101xx.h"

#define ICP_I2C_ID 0x63

#define ICP_CMD_READ_ID 0xefc8
#define ICP_CMD_SET_ADDR 0xc595
#define ICP_CMD_READ_OTP 0xc7f7
#define ICP_CMD_MEAS_LP 0x609c
#define ICP_CMD_MEAS_N 0x6825
#define ICP_CMD_MEAS_LN 0x70df
#define ICP_CMD_MEAS_ULN 0x7866

// constants for presure calculation
const float _pcal[3] = { 45000.0, 80000.0, 105000.0 };
const float _lut_lower = 3.5 * 0x100000;	// 1<<20
const float _lut_upper = 11.5 * 0x100000;	// 1<<20
const float _quadr_factor = 1 / 16777216.0;
const float _offst_factor = 2048.0;

ICP101xx::ICP101xx() {
	_i2c = NULL;
	_pressure_Pa = 0.0;
	_temperature_C = 0.0;
	_meas_duration = 100;
	_meas_start = millis();
	_data_ready = false;
}

bool ICP101xx::begin(TwoWire* wire) {
	// setup I2C object
	if (wire) {
		_i2c = wire;
	} else {
		_i2c = &Wire;
	}
	_i2c->begin();

	// verify that the sensor is repsonding
	if (!isConnected()) {
		return false;
	}

	// read sensor calibration data
	uint8_t addr_otp_cmd[5] = { 
				(ICP_CMD_SET_ADDR >> 8) & 0xff,
				ICP_CMD_SET_ADDR & 0xff,
				0x00, 0x66, 0x9c };
	uint8_t otp_buf[3];
	_sendCommand(addr_otp_cmd, 5);
	for (int i=0; i<4; i++) {
		_sendCommand(ICP_CMD_READ_OTP);
		_readResponse(otp_buf, 3);
		_scal[i] = (otp_buf[0] << 8) | otp_buf[1];
	}
	return true;
}

bool ICP101xx::isConnected(void) {
	uint8_t id_buf[2];
	_sendCommand(ICP_CMD_READ_ID);
	_readResponse(id_buf, 2);
	uint16_t id = (id_buf[0] << 8) | id_buf[1];
	if ((id & 0x03f) == 0x08) {
		return true;
	} else {
		return false;
	}
}

void ICP101xx::measure(ICP101xx::mmode mode) {
	delay(measureStart(mode));
	while (!dataReady());
}

uint8_t ICP101xx::measureStart(ICP101xx::mmode mode) {
	uint16_t cmd;
	switch (mode) {
	case ICP101xx::FAST:
		cmd = ICP_CMD_MEAS_LP;
		_meas_duration = 3;
		break;
	case ICP101xx::ACCURATE:
		cmd = ICP_CMD_MEAS_LN;
		_meas_duration = 24;
		break;
	case ICP101xx::VERY_ACCURATE:
		cmd = ICP_CMD_MEAS_ULN;
		_meas_duration = 95;
		break;
	case ICP101xx::NORMAL:
	default:
		cmd = ICP_CMD_MEAS_N;
		_meas_duration = 7;
		break;
	}
	_sendCommand(cmd);
	_data_ready = false;
	_meas_start = millis();
	return _meas_duration;
}

bool ICP101xx::dataReady(void) {
	if (_data_ready)
		return true;

	if (millis() - _meas_start < _meas_duration)
		return false;

	uint8_t res_buf[9];
	_readResponse(res_buf, 9);
	_raw_t = (res_buf[0] << 8) | res_buf[1];
	uint32_t L_res_buf3 = res_buf[3];	// expand result bytes to 32bit to fix issues on 8-bit MCUs
	uint32_t L_res_buf4 = res_buf[4];
	uint32_t L_res_buf6 = res_buf[6];
	_raw_p = (L_res_buf3 << 16) | (L_res_buf4 << 8) | L_res_buf6;
	_calculate();

	_data_ready = true;

	return true;
}

float ICP101xx::getTemperatureC(void) {
	return _temperature_C;
}

float ICP101xx::getTemperatureF(void) {
	return _temperature_C * 1.8 + 32;
}

float ICP101xx::getPressurePa(void) {
	return _pressure_Pa;
}

void ICP101xx::_calculate(void) {
	// calculate temperature
	_temperature_C = -45.f + 175.f / 65536.f * _raw_t;

	// calculate pressure
	float t = (float)(_raw_t - 32768);
	float s1 = _lut_lower + (float)(_scal[0] * t * t) * _quadr_factor;
	float s2 = _offst_factor * _scal[3] + (float)(_scal[1] * t * t) * _quadr_factor;
	float s3 = _lut_upper + (float)(_scal[2] * t * t) * _quadr_factor;
	float c = (s1 * s2 * (_pcal[0] - _pcal[1]) +
			   s2 * s3 * (_pcal[1] - _pcal[2]) +
			   s3 * s1 * (_pcal[2] - _pcal[0])) /
			  (s3 * (_pcal[0] - _pcal[1]) +
			   s1 * (_pcal[1] - _pcal[2]) +
			   s2 * (_pcal[2] - _pcal[0]));
	float a = (_pcal[0] * s1 - _pcal[1] * s2 - (_pcal[1] - _pcal[0]) * c) / (s1 - s2);
	float b = (_pcal[0] - a) * (s1 + c);
	_pressure_Pa = a + b / (c + _raw_p);
}

void ICP101xx::_sendCommand(uint16_t cmd) {
	if (!_i2c) return;

	_i2c->beginTransmission(ICP_I2C_ID);
	_i2c->write((cmd >> 8) & 0xff);
	_i2c->write(cmd & 0xff);
	_i2c->endTransmission();
}

void ICP101xx::_sendCommand(uint8_t *cmd_buf, uint8_t cmd_len) {
	if (!_i2c) return;

	if (cmd_buf && cmd_len) {
		_i2c->beginTransmission(ICP_I2C_ID);
		for (int i=0; i<cmd_len; i++) {
			_i2c->write(cmd_buf[i]);
		}
		_i2c->endTransmission();
	}
}

void ICP101xx::_readResponse(uint8_t *res_buf, uint8_t res_len) {
	if (!_i2c) return;

	if (res_buf && res_len) {
		_i2c->requestFrom(ICP_I2C_ID, res_len);
		for (int i=0; i<res_len; i++) {
			res_buf[i] = _i2c->read();
		}
	}
}