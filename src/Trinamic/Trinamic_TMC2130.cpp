/**************************************************************************/
/*!
@file     Trinamic_TMC2130.cpp
@author   Moritz Walter
@license  GPLv3 (see license.txt)

SPI configuration tool for Trinamic TMC2130 Motor Drivers

@section  HISTORY

v0.1 - it works
*/
/**************************************************************************/
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>

#include "Trinamic_TMC2130.h"
#include "Trinamic_TMC2130_registers.h"

Trinamic_TMC2130::Trinamic_TMC2130(uint8_t csPin)
{
  _csPin=csPin;
  _status=0;
  _debug="";
}

// initialize the driver with its CS/SS pin
void Trinamic_TMC2130::init() {
  pinMode(_csPin, OUTPUT);
  digitalWrite(_csPin, HIGH);
  init_SPI();
  read_STAT();
}

// initialize SPI
void Trinamic_TMC2130::init_SPI() {
  SPI.setDataMode(TMC_SPI_DATA_MODE);
  SPI.setBitOrder(TMC_SPI_BIT_ORDER);
  SPI.setClockDivider(TMC_SPI_CLOCK_DIVIDER);
  SPI.begin();
}

// read status
uint8_t Trinamic_TMC2130::read_STAT()
{
  init_SPI();
  digitalWrite(_csPin, LOW);

  // read address
  _status = SPI.transfer(0x00);

  // flush 4 bytes
  for(int i=0;i<4;i++){
    SPI.transfer(0x00);
  }

  digitalWrite(_csPin, HIGH);

  return _status;
}

// read a register
uint8_t Trinamic_TMC2130::read_REG(uint8_t address, uint32_t *data)
{
  init_SPI();
  digitalWrite(_csPin, LOW);

  // read address
  _status = SPI.transfer(address&~TMC_WRITE);

  // flush 4 bytes
  for(int i=0;i<4;i++){
    SPI.transfer(0x00);
  }

  digitalWrite(_csPin, HIGH);
  // restart transmission
  digitalWrite(_csPin, LOW);

  // read address
  _status = SPI.transfer(address&~TMC_WRITE);

  // retrieve data
  *data  = SPI.transfer(0x00)&0xFF;
  *data <<=8;
  *data |= SPI.transfer(0x00)&0xFF;
  *data <<=8;
  *data |= SPI.transfer(0x00)&0xFF;
  *data <<=8;
  *data |= SPI.transfer(0x00)&0xFF;

  digitalWrite(_csPin, HIGH);

  return _status;
}

// write to a register
uint8_t Trinamic_TMC2130::write_REG(uint8_t address, uint32_t data)
{
  digitalWrite(_csPin, LOW);

  // write address
  _status = SPI.transfer(address|TMC_WRITE);

  // write data
  SPI.transfer((data>>24UL)&0xFF);
  SPI.transfer((data>>16UL)&0xFF);
  SPI.transfer((data>> 8UL)&0xFF);
  SPI.transfer((data>> 0UL)&0xFF);

  digitalWrite(_csPin, HIGH);

  return _status;
}

// alter a register using a bitmask
uint8_t Trinamic_TMC2130::alter_REG(uint8_t address, uint32_t data, uint32_t mask)
{
  uint32_t oldData, newData;

  read_REG( address, &oldData );

  newData = ( oldData&~mask ) | ( data&mask );

  write_REG( address, newData );

  return _status;
}

// set single bits in the GCONF register
uint8_t Trinamic_TMC2130::set_GCONF(uint8_t position, uint8_t value)
{
  alter_REG(TMC_REG_GCONF, uint32_t(value)<<position, 0x1UL<<position);

  return _status;
}

// set single bits or values in the chopconf register (constraining masks are applied if necessary)
uint8_t Trinamic_TMC2130::set_CHOPCONF(uint8_t position, uint8_t value)
{
  alter_REG(TMC_REG_CHOPCONF, uint32_t(value)<<position, TMC_CHOPCONF_MASKS[position]<<position);

  return _status;
}

  //////////
 // GCONF
//////////

uint8_t Trinamic_TMC2130::set_I_scale_analog(uint8_t value)
{
  set_GCONF(TMC_GCONF_I_SCALE_ANALOG, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_internal_Rsense(uint8_t value)
{
  set_GCONF(TMC_GCONF_INTERNAL_RSENSE, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_en_pwm_mode(uint8_t value)
{
  set_GCONF(TMC_GCONF_EN_PWM_MODE, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_enc_commutation(uint8_t value)
{
  set_GCONF(TMC_GCONF_ENC_COMMUTATION, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_shaft(uint8_t value)
{
  set_GCONF(TMC_GCONF_SHAFT, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_diag0_error(uint8_t value)
{
  set_GCONF(TMC_GCONF_DIAG0_ERROR, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_diag0_otpw(uint8_t value)
{
  set_GCONF(TMC_GCONF_DIAG0_OTPW, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_diag0_stall(uint8_t value)
{
  set_GCONF(TMC_GCONF_DIAG0_STALL, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_diag1_stall(uint8_t value)
{
  set_GCONF(TMC_GCONF_DIAG1_STALL, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_diag1_index(uint8_t value)
{
  set_GCONF(TMC_GCONF_DIAG1_INDEX, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_diag1_onstate(uint8_t value)
{
  set_GCONF(TMC_GCONF_DIAG1_ONSTATE, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_diag1_steps_skipped(uint8_t value)
{
  set_GCONF(TMC_GCONF_DIAG1_STEPS_SKIPPED, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_diag0_int_pushpull(uint8_t value)
{
  set_GCONF(TMC_GCONF_DIAG0_INT_PUSHPULL, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_diag1_int_pushpull(uint8_t value)
{
  set_GCONF(TMC_GCONF_DIAG1_INT_PUSHPULL, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_small_hysteresis(uint8_t value)
{
  set_GCONF(TMC_GCONF_SMALL_HYSTERESIS, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_stop_enable(uint8_t value)
{
  set_GCONF(TMC_GCONF_STOP_ENABLE, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_direct_mode(uint8_t value)
{
  set_GCONF(TMC_GCONF_DIRECT_MODE, value);

  return _status;
}

/*
uint8_t Trinamic_TMC2130::set_test_mode(uint8_t value)
{
  set_GCONF(TMC_GCONF_TEST_MODE, value);

  return _status;
}
*/

//////////
// IHOLD_IRUN
//////////

uint8_t Trinamic_TMC2130::set_IHOLD_IRUN(uint8_t ihold, uint8_t irun, uint8_t iholddelay)
{
  uint32_t data;
  // adding ihold
  data = (( uint32_t(ihold)&TMC_IHOLD_MASK )<<TMC_IHOLD );
  // adding irun
  data |= (( uint32_t(irun)&TMC_IRUN_MASK )<<TMC_IRUN );
  // adding iholddelay
  data |= (( uint32_t(iholddelay)&TMC_IHOLDDELAY_MASK )<<TMC_IHOLDDELAY );

  // writing data
  write_REG(TMC_REG_IHOLD_IRUN, data);

  return _status;
}

//////////
// TPOWERDOWN
//////////

uint8_t Trinamic_TMC2130::set_TPOWERDOWN(uint8_t value)
{
  uint32_t data;

  data = value & TMC_TPOWERDOWN_MASK;

  write_REG(TMC_REG_TPOWERDOWN, data);

  return _status;
}

//////////
// TSTEP
//////////

uint32_t Trinamic_TMC2130::get_TSTEP()
{
  uint32_t data;

  read_REG(TMC_REG_TPOWERDOWN, &data);

  data &= TMC_TSTEP_MASK;

  return data;
}

//////////
// TPWMTHRS
//////////

uint8_t Trinamic_TMC2130::set_TPWMTHRS(uint32_t value)
{
  uint32_t data;

  data = value & TMC_TPWMTHRS_MASK;

  write_REG(TMC_REG_TPOWERDOWN, data);

  return _status;
}

//////////
// TCOOLTHRS
//////////

uint8_t Trinamic_TMC2130::set_TCOOLTHRS(uint32_t value)
{
  uint32_t data;

  data = value & TMC_TCOOLTHRS_MASK;

  write_REG(TMC_REG_TCOOLTHRS, data);

  return _status;
}

//////////
// THIGH
//////////

uint8_t Trinamic_TMC2130::set_THIGH(uint32_t value)
{
  uint32_t data;

  data = value & TMC_THIGH_MASK;

  write_REG(TMC_REG_THIGH, data);

  return _status;
}

//////////
// XDIRECT
//////////

uint8_t Trinamic_TMC2130::set_XDIRECT(int16_t coil_a, int16_t coil_b)
{
  uint32_t data;

  data = 0x0;
  data |= ( coil_b & TMC_XDIRECT_COIL_B_MASK );
  data = data << TMC_XDIRECT_COIL_B;
  data |= ( coil_a & TMC_XDIRECT_COIL_A_MASK );
  data &= TMC_XDIRECT_MASK;

  write_REG(TMC_REG_XDIRECT, data);

  return _status;
}

uint8_t Trinamic_TMC2130::set_XDIRECT(uint32_t value)
{
  uint32_t data;

  data = value;// & TMC_XDIRECT_MASK;

  write_REG(TMC_REG_XDIRECT, data);

  return _status;
}

int32_t Trinamic_TMC2130::get_XDIRECT()
{
  uint32_t data;

  read_REG(TMC_REG_XDIRECT, &data);

  data &= TMC_XDIRECT_MASK;

  return data;
}

//////////
// VDCMIN
//////////

uint8_t Trinamic_TMC2130::set_VDCMIN(int32_t value)
{
  int32_t data;

  data = value & TMC_VDCMIN_MASK;

  write_REG(TMC_REG_VDCMIN, data);

  return _status;
}

//////////
// MSLUT
//////////

uint8_t Trinamic_TMC2130::set_MSLUT0(uint32_t value)
{
  write_REG(TMC_REG_MSLUT0, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_MSLUT1(uint32_t value)
{
  write_REG(TMC_REG_MSLUT1, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_MSLUT2(uint32_t value)
{
  write_REG(TMC_REG_MSLUT2, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_MSLUT3(uint32_t value)
{
  write_REG(TMC_REG_MSLUT3, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_MSLUT4(uint32_t value)
{
  write_REG(TMC_REG_MSLUT4, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_MSLUT5(uint32_t value)
{
  write_REG(TMC_REG_MSLUT5, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_MSLUT6(uint32_t value)
{
  write_REG(TMC_REG_MSLUT6, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_MSLUT7(uint32_t value)
{
  write_REG(TMC_REG_MSLUT7, value);

  return _status;
}

//////////
// MSLUTSEL
//////////

uint8_t Trinamic_TMC2130::set_MSLUTSEL(uint32_t value)
{
  write_REG(TMC_REG_MSLUTSEL, value);

  return _status;
}

//////////
// MSLUTSTART
//////////

uint8_t Trinamic_TMC2130::set_MSLUTSTART(uint8_t start_sin, uint8_t start_sin90)
{
  uint32_t data;
  data = ( uint32_t(start_sin90) & TMC_MSLUTSTART_START_SIN90_MASK );
  data = data<<TMC_MSLUTSTART_START_SIN90;
  data |= ( uint32_t(start_sin) & TMC_MSLUTSTART_START_SIN_MASK );
  data &= TMC_MSLUTSTART_MASK;

  write_REG(TMC_REG_MSLUTSTART, data);

  return _status;
}

//////////
// MSCNT
//////////

uint16_t Trinamic_TMC2130::get_MSCNT()
{
  uint32_t data;

  read_REG(TMC_REG_MSCNT, &data);

  data &= TMC_MSCNT_MASK;

  data = uint16_t(data);

  return data;
}

//////////
// MSCURACT
//////////

int32_t Trinamic_TMC2130::get_MSCURACT()
{
  uint32_t data;

  read_REG(TMC_REG_MSCURACT, &data);

  data &= TMC_MSCURACT_MASK;

  return data;
}

//////////
// CHOPCONF
//////////

uint8_t Trinamic_TMC2130::set_diss2g(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_DISS2G, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_dedge(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_DEDGE, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_intpol(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_INTPOL, value);

  return _status;
}

// setting the microstep resolution
uint8_t Trinamic_TMC2130::set_mres(uint16_t value)
{
  uint8_t data = 0;

  switch(value){
    case 1:
      data = 8;
    break;
    case 2:
      data = 7;
    break;
    case 4:
      data = 6;
    break;
    case 8:
      data = 5;
    break;
    case 16:
      data = 4;
    break;
    case 32:
      data = 3;
    break;
    case 64:
      data = 2;
    break;
    case 128:
      data = 1;
    break;
  }

  set_CHOPCONF(TMC_CHOPCONF_MRES, data);

  return _status;
}

uint8_t Trinamic_TMC2130::set_sync(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_SYNC, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_vhighchm(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_VHIGHCHM, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_vhighfs(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_VHIGHFS, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_vsense(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_VSENSE, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_tbl(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_TBL, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_chm(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_CHM, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_rndtf(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_RNDTF, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_disfdcc(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_DISFDCC, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_fd(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_FD, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_hend(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_HEND, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_hstrt(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_HSTRT, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_toff(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_TOFF, value);

  return _status;
}

  //////////
 // COOLCONF
//////////

// alter coolconf
uint8_t Trinamic_TMC2130::alter_COOLCONF(uint32_t data, uint32_t mask)
{
  uint32_t newData;

  _coolconf = ( _coolconf & ~mask ) | ( data & mask );

  write_REG( TMC_REG_COOLCONF, _coolconf );

  return _status;
}

// set coolconf
uint8_t Trinamic_TMC2130::set_COOLCONF(uint8_t position, uint8_t value)
{

  alter_COOLCONF( uint32_t(value)<<position, TMC_CHOPCONF_MASKS[position]<<position);

  return _status;
}

uint8_t Trinamic_TMC2130::set_sfilt(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SFILT, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_sgt(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SGT, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_seimin(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SEIMIN, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_sedn(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SEDN, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_semax(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SEMAX, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_seup(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SEUP, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_semin(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SEMIN, value);

  return _status;
}

  //////////
 // DCCTRL
//////////

uint8_t Trinamic_TMC2130::set_DCCTRL(uint16_t dc_time, uint16_t dc_sg)
{
  uint32_t data;
  data = ( uint32_t(dc_sg) & TMC_DCCTRL_DC_SG_MASK );
  data = data<<TMC_DCCTRL_DC_SG;
  data |= ( uint32_t(dc_time) & TMC_DCCTRL_DC_TIME_MASK );
  data &= TMC_DCCTRL_MASK;

  write_REG(TMC_REG_DCCTRL, data);

  return _status;
}

  //////////
 // PWMCONF
//////////

// alter pwmconf
uint8_t Trinamic_TMC2130::alter_PWMCONF(uint32_t data, uint32_t mask)
{
  uint32_t newData;

  _pwmconf = ( _pwmconf & ~mask ) | ( data & mask );

  write_REG( TMC_REG_PWMCONF, _pwmconf );

  return _status;
}

// set pwmconf
uint8_t Trinamic_TMC2130::set_PWMCONF(uint8_t position, uint8_t value)
{

  alter_PWMCONF( uint32_t(value)<<position, TMC_CHOPCONF_MASKS[position]<<position );

  return _status;
}

uint8_t Trinamic_TMC2130::set_freewheel(uint8_t value)
{
  set_PWMCONF(TMC_PWMCONF_FREEWHEEL, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_pwm_symmetric(uint8_t value)
{
  set_PWMCONF(TMC_PWMCONF_PWM_SYMMETRIC, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_pwm_autoscale(uint8_t value)
{
  set_PWMCONF(TMC_PWMCONF_PWM_AUTOSCALE, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_pwm_freq(uint8_t value)
{
  set_PWMCONF(TMC_PWMCONF_PWM_FREQ, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_PWM_GRAD(uint8_t value)
{
  set_PWMCONF(TMC_PWMCONF_PWM_GRAD, value);

  return _status;
}

uint8_t Trinamic_TMC2130::set_PWM_AMPL(uint8_t value)
{
  set_PWMCONF(TMC_PWMCONF_PWM_AMPL, value);

  return _status;
}

  //////////
 // ENCM_CTRL
//////////

uint8_t Trinamic_TMC2130::set_ENCM_CTRL(uint8_t value)
{
  uint8_t data;

  data = value & TMC_ENCM_CTRL_MASK;

  write_REG(TMC_REG_ENCM_CTRL, data);

  return _status;
}

  //////////
 // STATUS
//////////

// check the reset status
boolean Trinamic_TMC2130::isReset()
{
  return _status&TMC_SPISTATUS_RESET_MASK ? true : false;
}

// check the error status
boolean Trinamic_TMC2130::isError()
{
  return _status&TMC_SPISTATUS_ERROR_MASK ? true : false;
}

// check the stallguard status
boolean Trinamic_TMC2130::isStallguard()
{
  return _status&TMC_SPISTATUS_STALLGUARD_MASK ? true : false;
}

// check the standstill status
boolean Trinamic_TMC2130::isStandstill()
{
  return _status&TMC_SPISTATUS_STANDSTILL_MASK ? true : false;
}

// get debug messages
String Trinamic_TMC2130::debug()
{
  return _debug;
}
