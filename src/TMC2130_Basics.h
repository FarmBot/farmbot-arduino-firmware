// TMC2130_Basics.h

#ifndef _TMC2130_BASICS_h
#define _TMC2130_BASICS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "TMC2130_Basics_Reg.h"

class TMC2130_Basics {
public:
  TMC2130_Basics(uint8_t csPin, uint8_t axisNumber);
  void init();
  // void init_SPI();

  uint8_t read_STAT();
  uint8_t read_REG(uint8_t address, uint32_t *data);
  uint8_t write_REG(uint8_t address, uint32_t data);
  uint8_t alter_REG(uint8_t address, uint32_t data, uint32_t mask);

  uint8_t set_GCONF(uint8_t bit, uint8_t value);
  uint8_t set_CHOPCONF(uint8_t position, uint8_t value);

  uint8_t getStatus();
  uint8_t getAxisNumber();

  //boolean isReset();
  //boolean isError();
  boolean isStallguard();
  boolean isStandstill();

  //String debug();

private:
  uint32_t _coolconf;
  uint32_t _pwmconf;
  uint8_t _csPin;
  uint8_t _status;
  uint8_t _axisNumber;
  //String _debug;
};



#endif

