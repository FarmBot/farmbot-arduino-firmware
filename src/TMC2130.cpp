/*
 * TMC2130.cpp
 *
 *  Created on: 03 nov 2019
 *      Author: Tim Evers
 */

#include "TMC2130.h"

/*
static TMC2130Stepper *TMC2130X;
static TMC2130Stepper *TMC2130Y;
static TMC2130Stepper *TMC2130Z;
static TMC2130Stepper *TMC2130E;
*/

/*
static TMC2130Stepper TMC2130_X = TMC2130Stepper(X_ENABLE_PIN, X_DIR_PIN, X_STEP_PIN, X_CHIP_SELECT);
static TMC2130Stepper TMC2130_Y = TMC2130Stepper(Y_ENABLE_PIN, Y_DIR_PIN, Y_STEP_PIN, Y_CHIP_SELECT);
static TMC2130Stepper TMC2130_Z = TMC2130Stepper(Z_ENABLE_PIN, Z_DIR_PIN, Z_STEP_PIN, Z_CHIP_SELECT);
static TMC2130Stepper TMC2130_E = TMC2130Stepper(E_ENABLE_PIN, E_DIR_PIN, E_STEP_PIN, E_CHIP_SELECT);
*/

/*
static TMC2130Holder *instance;

TMC2130Holder *TMC2130Holder::getInstance()
{
  if (!instance)
  {
    instance = new TMC2130Holder();
  };
  return instance;
};

TMC2130Holder::TMC2130Holder()
{
}

void TMC2130Holder::loadDrivers()
{
  TMC2130Stepper TMC2130X = new TMC2130Stepper(X_ENABLE_PIN, X_DIR_PIN, X_STEP_PIN, X_CHIP_SELECT);
  TMC2130Stepper TMC2130Y = new TMC2130Stepper(Y_ENABLE_PIN, Y_DIR_PIN, Y_STEP_PIN, Y_CHIP_SELECT);
  TMC2130Stepper TMC2130Z = new TMC2130Stepper(Z_ENABLE_PIN, Z_DIR_PIN, Z_STEP_PIN, Z_CHIP_SELECT);
  TMC2130Stepper TMC2130E = new TMC2130Stepper(E_ENABLE_PIN, E_DIR_PIN, E_STEP_PIN, E_CHIP_SELECT);

  tmcTMC2130X = &TMC2130X;
  tmcTMC2130Y = &TMC2130Y;
  tmcTMC2130Z = &TMC2130Z;
  tmcTMC2130E = &TMC2130E;

}


TMC2130Stepper* TMC2130Holder::TMC2130X()
{
  return tmcTMC2130X;
}

TMC2130Stepper* TMC2130Holder::TMC2130Y()
{
  return tmcTMC2130Y;
}

TMC2130Stepper* TMC2130Holder::TMC2130Z()
{
  return tmcTMC2130Z;
}
c
TMC2130Stepper* TMC2130Holder::TMC2130E()
{
  return tmcTMC213EZ;
}
*/