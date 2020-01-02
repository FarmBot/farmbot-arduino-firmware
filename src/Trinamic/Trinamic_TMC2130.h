/**************************************************************************/
/*!
@file     Trinamic_TMC2130.h
@author   Moritz Walter
@license  GPLv3 (see license.txt)

SPI configuration tool for Trinamic TMC2130 Motor Drivers

@section  HISTORY

v0.1 - it works
*/
/**************************************************************************/
#ifndef TRINAMIC_TMC2130_H
#define TRINAMIC_TMC2130_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>

#include "Trinamic_TMC2130_registers.h"

class Trinamic_TMC2130{
public:
  Trinamic_TMC2130(uint8_t csPin);
  void init();
  void init_SPI();

  uint8_t read_STAT();
  uint8_t read_REG( uint8_t address , uint32_t *data );
  uint8_t write_REG( uint8_t address, uint32_t data );
  uint8_t alter_REG(uint8_t address, uint32_t data, uint32_t mask);

  uint8_t set_GCONF(uint8_t bit, uint8_t value);
  uint8_t set_I_scale_analog(uint8_t value);
  uint8_t set_internal_Rsense(uint8_t value);
  uint8_t set_en_pwm_mode(uint8_t value);
  uint8_t set_enc_commutation(uint8_t value);
  uint8_t set_shaft(uint8_t value);
  uint8_t set_diag0_error(uint8_t value);
  uint8_t set_diag0_otpw(uint8_t value);
  uint8_t set_diag0_stall(uint8_t value);
  uint8_t set_diag1_stall(uint8_t value);
  uint8_t set_diag1_index(uint8_t value);
  uint8_t set_diag1_onstate(uint8_t value);
  uint8_t set_diag1_steps_skipped(uint8_t value);
  uint8_t set_diag0_int_pushpull(uint8_t value);
  uint8_t set_diag1_int_pushpull(uint8_t value);
  uint8_t set_small_hysteresis(uint8_t value);
  uint8_t set_stop_enable(uint8_t value);
  uint8_t set_direct_mode(uint8_t value);
  //uint8_t test_mode(uint8_t value);

  uint8_t set_IHOLD_IRUN(uint8_t ihold, uint8_t irun, uint8_t iholddelay);
  uint8_t set_TPOWERDOWN(uint8_t value);
  uint32_t get_TSTEP();
  uint8_t set_TPWMTHRS(uint32_t value);
  uint8_t set_TCOOLTHRS(uint32_t value);
  uint8_t set_THIGH(uint32_t value);
  uint8_t set_XDIRECT(uint32_t value);
  uint8_t set_XDIRECT(int16_t coil_a, int16_t coil_b);
  int32_t get_XDIRECT();
  uint8_t set_VDCMIN(int32_t value);

  uint8_t set_MSLUT0(uint32_t value);
  uint8_t set_MSLUT1(uint32_t value);
  uint8_t set_MSLUT2(uint32_t value);
  uint8_t set_MSLUT3(uint32_t value);
  uint8_t set_MSLUT4(uint32_t value);
  uint8_t set_MSLUT5(uint32_t value);
  uint8_t set_MSLUT6(uint32_t value);
  uint8_t set_MSLUT7(uint32_t value);
  uint8_t set_MSLUTSEL(uint32_t value);
  uint8_t set_MSLUTSTART(uint8_t start_sin, uint8_t start_sin90);
  uint16_t get_MSCNT();
  int32_t get_MSCURACT();

  uint8_t set_CHOPCONF(uint8_t position, uint8_t value);
  uint8_t set_dedge(uint8_t value);
  uint8_t set_diss2g(uint8_t value);
  uint8_t set_intpol(uint8_t value);
  uint8_t set_mres(uint16_t value);
  uint8_t set_sync(uint8_t value);
  uint8_t set_vhighchm(uint8_t value);
  uint8_t set_vhighfs(uint8_t value);
  uint8_t set_vsense(uint8_t value);
  uint8_t set_tbl(uint8_t value);
  uint8_t set_chm(uint8_t value);
  uint8_t set_rndtf(uint8_t value);
  uint8_t set_disfdcc(uint8_t value);
  uint8_t set_fd(uint8_t value);
  uint8_t set_hend(uint8_t value);
  uint8_t set_hstrt(uint8_t value);
  uint8_t set_toff(uint8_t value);

  uint8_t alter_COOLCONF(uint32_t data, uint32_t mask);
  uint8_t set_COOLCONF(uint8_t position, uint8_t value);
  uint8_t set_sfilt(uint8_t value);
  uint8_t set_sgt(uint8_t value);
  uint8_t set_seimin(uint8_t value);
  uint8_t set_sedn(uint8_t value);
  uint8_t set_semax(uint8_t value);
  uint8_t set_seup(uint8_t value);
  uint8_t set_semin(uint8_t value);

  uint8_t set_DCCTRL(uint16_t dc_time, uint16_t dc_sg);

  uint8_t alter_PWMCONF(uint32_t data, uint32_t mask);
  uint8_t set_PWMCONF(uint8_t position, uint8_t value);
  uint8_t set_freewheel(uint8_t value);
  uint8_t set_pwm_symmetric(uint8_t value);
  uint8_t set_pwm_autoscale(uint8_t value);
  uint8_t set_pwm_freq(uint8_t value);
  uint8_t set_PWM_GRAD(uint8_t value);
  uint8_t set_PWM_AMPL(uint8_t value);

  uint8_t set_ENCM_CTRL(uint8_t value);

  boolean isReset();
  boolean isError();
  boolean isStallguard();
  boolean isStandstill();
  String debug();

private:
  uint32_t _coolconf;
  uint32_t _pwmconf;
  uint8_t _csPin;
  uint8_t _status;
  String _debug;
};

#endif // TRINAMIC_TMC2130_H
