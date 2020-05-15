/*
 * TMC2130.cpp
 *
 *  Created on: 03 nov 2019
 *      Author: Tim Evers
 */


#include "TMC2130.h"

#if defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_V30)

//void loadTMC2130ParametersMotor(Trinamic_TMC2130 *myStepper, int microsteps, int current, int sensitivity)
void loadTMC2130ParametersMotor(TMC2130_Basics *tb, int microsteps, int current, int sensitivity)
{

  //Serial.print("==>");
  //Serial.print(" ");
  //Serial.print(microsteps);
  //Serial.print(" ");
  //Serial.print(current);
  //Serial.print(" ");
  //Serial.print(sensitivity);
  //Serial.println(" ");

  /*
  tb->init();

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
  }

  tb->set_CHOPCONF(FB_TMC_CHOPCONF_TOFF, 3);
  tb->set_CHOPCONF(FB_TMC_CHOPCONF_TBL, 1);
  tb->set_GCONF(FB_TMC_GCONF_DIAG1_STALL, 1);
  tb->set_GCONF(FB_TMC_GCONF_DIAG1_ONSTATE, 1);
  {
    uint32_t data;

    data = 0xFFFFF & FB_TMC_TCOOLTHRS_MASK;

    tb->write_REG(FB_TMC_REG_TCOOLTHRS, data);
  }
  {
    uint32_t data;

    data = 0 & FB_TMC_THIGH_MASK;

    tb->write_REG(FB_TMC_REG_THIGH, data);
  }
  {
    tb->set_CHOPCONF(FB_TMC_COOLCONF_SEMIN, 5);
  }
  tb->set_CHOPCONF(FB_TMC_COOLCONF_SEMAX, 2);
  tb->set_CHOPCONF(FB_TMC_COOLCONF_SEMAX, 0b01);
  tb->set_CHOPCONF(FB_TMC_COOLCONF_SGT, sensitivity);

  */

  uint32_t data = 0;
  uint32_t value = 0;

  tb->init();

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

  uint16_t mA = current;
  float multiplier = 0.5;
  float RS = 0.11;
  uint8_t CS = 32.0*1.41421*mA / 1000.0*(RS + 0.02) / 0.325 - 1;
  if (CS < 16) {
    CS = 32.0*1.41421*mA / 1000.0*(RS + 0.02) / 0.180 - 1;
  }
  data = ((uint32_t(CS)&FB_TMC_IHOLD_MASK) << FB_TMC_IHOLD);
  data |= ((uint32_t(CS)&FB_TMC_IRUN_MASK) << FB_TMC_IRUN);
  data |= ((uint32_t(16)&FB_TMC_IHOLDDELAY_MASK) << FB_TMC_IHOLDDELAY);
  tb->write_REG(FB_TMC_REG_IHOLD_IRUN, data);

  tb->alter_REG(FB_TMC_REG_CHOPCONF, uint32_t(sensitivity) << FB_TMC_COOLCONF_SGT, FB_TMC_CHOPCONF_MASKS[FB_TMC_COOLCONF_SGT] << FB_TMC_COOLCONF_SGT);

  tb->set_GCONF(FB_TMC_GCONF_I_SCALE_ANALOG, 1);
  tb->alter_REG(FB_TMC_REG_CHOPCONF, uint32_t(3) << FB_TMC_CHOPCONF_TOFF, FB_TMC_CHOPCONF_MASKS[FB_TMC_CHOPCONF_TOFF] << FB_TMC_CHOPCONF_TOFF);
  tb->alter_REG(FB_TMC_REG_CHOPCONF, uint32_t(1) << FB_TMC_CHOPCONF_TBL, FB_TMC_CHOPCONF_MASKS[FB_TMC_CHOPCONF_TBL] << FB_TMC_CHOPCONF_TBL);
  tb->set_GCONF(FB_TMC_GCONF_DIAG1_STALL, 1);
  tb->set_GCONF(FB_TMC_GCONF_DIAG1_ONSTATE, 1);

  tb->write_REG(FB_TMC_REG_TCOOLTHRS, 0xFFFFF & FB_TMC_TCOOLTHRS_MASK);
  tb->write_REG(FB_TMC_REG_THIGH, 0xFFFFF & FB_TMC_THIGH_MASK);
  tb->alter_REG(FB_TMC_REG_CHOPCONF, uint32_t(5) << FB_TMC_COOLCONF_SEMIN, FB_TMC_CHOPCONF_MASKS[FB_TMC_COOLCONF_SEMIN] << FB_TMC_COOLCONF_SEMIN);
  tb->alter_REG(FB_TMC_REG_CHOPCONF, uint32_t(15) << FB_TMC_COOLCONF_SEMAX, FB_TMC_CHOPCONF_MASKS[FB_TMC_COOLCONF_SEMAX] << FB_TMC_COOLCONF_SEMAX);

  /* // 2020-05-15 copy of settings before experiments
  tb->write_REG(FB_TMC_REG_TCOOLTHRS, 0xFFFFF & FB_TMC_TCOOLTHRS_MASK);
  tb->write_REG(FB_TMC_REG_THIGH, 0 & FB_TMC_THIGH_MASK);
  tb->alter_REG(FB_TMC_REG_CHOPCONF, uint32_t(5) << FB_TMC_COOLCONF_SEMIN, FB_TMC_CHOPCONF_MASKS[FB_TMC_COOLCONF_SEMIN] << FB_TMC_COOLCONF_SEMIN);
  tb->alter_REG(FB_TMC_REG_CHOPCONF, uint32_t(2) << FB_TMC_COOLCONF_SEMAX, FB_TMC_CHOPCONF_MASKS[FB_TMC_COOLCONF_SEMAX] << FB_TMC_COOLCONF_SEMAX);
  tb->alter_REG(FB_TMC_REG_CHOPCONF, uint32_t(0b01) << FB_TMC_COOLCONF_SEDN, FB_TMC_CHOPCONF_MASKS[FB_TMC_COOLCONF_SEDN] << FB_TMC_COOLCONF_SEDN);
  */

  delay(100);

}

#endif