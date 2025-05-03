/******************************************************************
 Library for the TDK InvenSense ICP-101xx series of barometric 
 pressure sensors. These sensors communicate over I2C.
 
 Tested with ICP-10100, but likely to also work with ICP-10101,
 ICP-10110 and ICP-10111.
 
 Written by Adrian Studer.
 
 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.
 ******************************************************************/
 
#ifndef _icp101xx
#define _icp101xx
 
#include <inttypes.h>
#include <Wire.h>

class ICP101xx
{
	public:
		enum mmode { FAST, NORMAL, ACCURATE, VERY_ACCURATE };

		ICP101xx();
		bool begin(TwoWire* wire = NULL);
		bool isConnected(void);
		void measure(ICP101xx::mmode mode = ICP101xx::NORMAL);
		uint8_t measureStart(ICP101xx::mmode mode = ICP101xx::NORMAL);
		bool dataReady();
		float getTemperatureC(void);
		float getTemperatureF(void);
		float getPressurePa(void);

	private:
		void _calculate(void);
		void _sendCommand(uint16_t cmd);
		void _sendCommand(uint8_t *cmd_buf, uint8_t cmd_len);
		void _readResponse(uint8_t *res_buf = NULL, uint8_t res_len = 0);

		TwoWire* _i2c;
		float _scal[4];
		uint16_t _raw_t;
		uint32_t _raw_p;
		float _temperature_C;
		float _pressure_Pa;
		unsigned long _meas_start;
		uint8_t _meas_duration;
		bool _data_ready;
};

#endif