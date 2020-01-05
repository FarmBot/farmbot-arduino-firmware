/*
 * TMC2130.cpp
 *
 *  Created on: 03 nov 2019
 *      Author: Tim Evers
 */

#include "TMC2130.h"

void loadTMC2130ParametersMotor(Trinamic_TMC2130 *myStepper, int microsteps, int current, int sensitivity)
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
  myStepper->init();

  myStepper->set_mres(microsteps); // ({1,2,4,8,16,32,64,128,256}) number of microsteps
  //myStepper->set_IHOLD_IRUN(31, 31, 5); // ([0-31],[0-31],[0-5]) sets all currents to maximum
  myStepper->set_I_scale_analog(1); // ({0,1}) 0: I_REF internal, 1: sets I_REF to AIN

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
    myStepper->set_IHOLD_IRUN(CS, CS, 16);
  }

  myStepper->set_toff(3); // ([0-15]) 0: driver disable, 1: use only with TBL>2, 2-15: off time setting during slow decay phase
  myStepper->set_tbl(1); // ([0-3]) set comparator blank time to 16, 24, 36 or 54 clocks, 1 or 2 is recommended

  // load settings needed for enabling stall guard
  myStepper->set_diag1_stall(1);
  myStepper->set_diag1_onstate(1);
  myStepper->set_TCOOLTHRS(0xFFFFF);
  myStepper->set_THIGH(0);
  myStepper->set_semin(5);
  myStepper->set_semax(2);
  myStepper->set_sedn(0b01);
  myStepper->set_sgt(sensitivity);

}