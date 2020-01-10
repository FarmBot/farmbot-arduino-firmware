/*
 * TMC2130.cpp
 *
 *  Created on: 03 nov 2019
 *      Author: Tim Evers
 */

#include "TMC2130.h"

//void loadTMC2130ParametersMotor(Trinamic_TMC2130 *myStepper, int microsteps, int current, int sensitivity)
void loadTMC2130ParametersMotor(TMC2130_Basics *tb, int microsteps, int current, int sensitivity)
{

  Serial.print("==>");
  Serial.print(" ");
  Serial.print(microsteps);
  Serial.print(" ");
  Serial.print(current);
  Serial.print(" ");
  Serial.print(sensitivity);
  Serial.println(" ");

  /**/
  //myStepper->init();
  tb->init();

  //myStepper->set_mres(microsteps); // ({1,2,4,8,16,32,64,128,256}) number of microsteps
  uint8_t data = 0;

  switch (microsteps) {
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
  tb->alter_REG(FB_TMC_REG_CHOPCONF, uint32_t(data) << FB_TMC_CHOPCONF_MRES, FB_TMC_CHOPCONF_MASKS[FB_TMC_CHOPCONF_MRES] << FB_TMC_CHOPCONF_MRES);

  //myStepper->set_I_scale_analog(1); // ({0,1}) 0: I_REF internal, 1: sets I_REF to AIN
  tb->set_GCONF(FB_TMC_GCONF_I_SCALE_ANALOG, 1);

  // load drive current for motors
  {
    uint16_t mA = current;
    float multiplier = 0.5;
    float RS = 0.11;
    uint8_t CS = 32.0*1.41421*mA / 1000.0*(RS + 0.02) / 0.325 - 1;
    // If Current Scale is too low, turn on high sensitivity R_sense and calculate again
    if (CS < 16) {
      CS = 32.0*1.41421*mA / 1000.0*(RS + 0.02) / 0.180 - 1;
    }
    {
      uint32_t data;
      // adding ihold
      data = ((uint32_t(CS)&FB_TMC_IHOLD_MASK) << FB_TMC_IHOLD);
      // adding irun
      data |= ((uint32_t(CS)&FB_TMC_IRUN_MASK) << FB_TMC_IRUN);
      // adding iholddelay
      data |= ((uint32_t(16)&FB_TMC_IHOLDDELAY_MASK) << FB_TMC_IHOLDDELAY);

      // writing data
      tb->write_REG(FB_TMC_REG_IHOLD_IRUN, data);
    }
    //myStepper->set_IHOLD_IRUN(CS, CS, 16);
  }

  //myStepper->set_toff(3); // ([0-15]) 0: driver disable, 1: use only with TBL>2, 2-15: off time setting during slow decay phase
  tb->set_CHOPCONF(FB_TMC_CHOPCONF_TOFF, 3);

  //myStepper->set_tbl(1); // ([0-3]) set comparator blank time to 16, 24, 36 or 54 clocks, 1 or 2 is recommended
  tb->set_CHOPCONF(FB_TMC_CHOPCONF_TBL, 1);

  // load settings needed for enabling stall guard
  //myStepper->set_diag1_stall(1);
  tb->set_GCONF(FB_TMC_GCONF_DIAG1_STALL, 1);

  //myStepper->set_diag1_onstate(1);
  tb->set_GCONF(FB_TMC_GCONF_DIAG1_ONSTATE, 1);

  //myStepper->set_TCOOLTHRS(0xFFFFF);
  {
    uint32_t data;

    data = 0xFFFFF & FB_TMC_TCOOLTHRS_MASK;

    tb->write_REG(FB_TMC_REG_TCOOLTHRS, data);
  }

  //myStepper->set_THIGH(0);
  {
    uint32_t data;

    data = 0 & FB_TMC_THIGH_MASK;

    tb->write_REG(FB_TMC_REG_THIGH, data);
  }

  //myStepper->set_semin(5);
  {
    tb->set_CHOPCONF(FB_TMC_COOLCONF_SEMIN, 5);
  }

  //myStepper->set_semax(2);
  tb->set_CHOPCONF(FB_TMC_COOLCONF_SEMAX, 2);

  //myStepper->set_sedn(0b01);
  tb->set_CHOPCONF(FB_TMC_COOLCONF_SEMAX, 0b01);

  //myStepper->set_sgt(sensitivity);
  tb->set_CHOPCONF(FB_TMC_COOLCONF_SGT, sensitivity);

}




