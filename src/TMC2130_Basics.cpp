#include "TMC2130_Basics.h"

TMC2130_Basics::TMC2130_Basics(uint8_t csPin, uint8_t axisNumber)
{
  _csPin = csPin;
  _status = 0;
  _axisNumber = axisNumber;
  //_debug = "";
}

// initialize the driver with its CS/SS pin
void TMC2130_Basics::init() {
  pinMode(_csPin, OUTPUT);
  digitalWrite(_csPin, HIGH);
  // init_SPI();
  read_STAT();
}

// initialize SPI
// void TMC2130_Basics::init_SPI() {
//   SPI.setDataMode(FB_TMC_SPI_DATA_MODE);
//   SPI.setBitOrder(FB_TMC_SPI_BIT_ORDER);
//   //SPI.setClockDivider(FB_TMC_SPI_CLOCK_DIVIDER);/**
//   SPI.setClockDivider(SPI_CLOCK_DIV4);
//   SPI.begin();
// }

// read status
uint8_t TMC2130_Basics::read_STAT()
{
  //init_SPI();
  SPI.beginTransaction(SPISettings(FB_TMC_SPI_CLOCK_DIVIDER, FB_TMC_SPI_BIT_ORDER, FB_TMC_SPI_DATA_MODE));
  digitalWrite(_csPin, LOW);

  // read address
  _status = SPI.transfer(0x00);

  // flush 4 bytes
  for (int i = 0; i < 4; i++) {
    SPI.transfer(0x00);
  }

  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();

  return _status;
}

// read a register
uint8_t TMC2130_Basics::read_REG(uint8_t address, uint32_t *data)
{
  // init_SPI();
  SPI.beginTransaction(SPISettings(FB_TMC_SPI_CLOCK_DIVIDER, FB_TMC_SPI_BIT_ORDER, FB_TMC_SPI_DATA_MODE));
  digitalWrite(_csPin, LOW);

  // read address
  _status = SPI.transfer(address&~FB_TMC_WRITE);

  // flush 4 bytes
  for (int i = 0; i < 4; i++) {
    SPI.transfer(0x00);
  }

  digitalWrite(_csPin, HIGH);
  // restart transmission
  digitalWrite(_csPin, LOW);

  // read address
  _status = SPI.transfer(address&~FB_TMC_WRITE);

  // retrieve data
  *data = SPI.transfer(0x00) & 0xFF;
  *data <<= 8;
  *data |= SPI.transfer(0x00) & 0xFF;
  *data <<= 8;
  *data |= SPI.transfer(0x00) & 0xFF;
  *data <<= 8;
  *data |= SPI.transfer(0x00) & 0xFF;

  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();

  return _status;
}

// write to a register
uint8_t TMC2130_Basics::write_REG(uint8_t address, uint32_t data)
{
  SPI.beginTransaction(SPISettings(FB_TMC_SPI_CLOCK_DIVIDER, FB_TMC_SPI_BIT_ORDER, FB_TMC_SPI_DATA_MODE));
  digitalWrite(_csPin, LOW);

  // write address
  _status = SPI.transfer(address | FB_TMC_WRITE);

  // write data
  SPI.transfer((data >> 24UL) & 0xFF);
  SPI.transfer((data >> 16UL) & 0xFF);
  SPI.transfer((data >> 8UL) & 0xFF);
  SPI.transfer((data >> 0UL) & 0xFF);

  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();

  return _status;
}

// alter a register using a bitmask
uint8_t TMC2130_Basics::alter_REG(uint8_t address, uint32_t data, uint32_t mask)
{
  uint32_t oldData, newData;

  read_REG(address, &oldData);

  newData = (oldData&~mask) | (data&mask);

  write_REG(address, newData);

  return _status;
}


// check the stallguard status
boolean TMC2130_Basics::isStallguard()
{
  return _status & FB_TMC_SPISTATUS_STALLGUARD_MASK ? true : false;
}

// check the standstill status
boolean TMC2130_Basics::isStandstill()
{
  return _status & FB_TMC_SPISTATUS_STANDSTILL_MASK ? true : false;
}


// set single bits in the GCONF register
uint8_t TMC2130_Basics::set_GCONF(uint8_t position, uint8_t value)
{
  alter_REG(FB_TMC_REG_GCONF, uint32_t(value) << position, 0x1UL << position);

  return _status;
}

// set single bits or values in the chopconf register (constraining masks are applied if necessary)
uint8_t TMC2130_Basics::set_CHOPCONF(uint8_t position, uint8_t value)
{
  alter_REG(FB_TMC_REG_CHOPCONF, uint32_t(value) << position, FB_TMC_CHOPCONF_MASKS[position] << position);

  return _status;
}

uint8_t TMC2130_Basics::getStatus()
{
  return _status;
}

uint8_t TMC2130_Basics::getAxisNumber()
{
  return _axisNumber;
}
