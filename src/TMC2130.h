/*
 * TMC2130.h
 *
 *  Created on: 03 nov 2019
 *      Author: Tim Evers
 */


#ifndef TMC2130_H_
#define TMC2130_H_

//#include <TMC2130Stepper.h>
//#include <TMC2130Stepper_REGDEFS.h>

#include "TMC2130/TMC2130Stepper.h"
#include "TMC2130/TMC2130Stepper_REGDEFS.h"

#include "pins.h"

static TMC2130Stepper *TMC2130X;
static TMC2130Stepper *TMC2130Y;
static TMC2130Stepper *TMC2130Z;
static TMC2130Stepper *TMC2130E;


//static TMC2130Stepper TMC2130_X = TMC2130Stepper(X_ENABLE_PIN, X_DIR_PIN, X_STEP_PIN, X_CHIP_SELECT);
//static TMC2130Stepper TMC2130_Y = TMC2130Stepper(Y_ENABLE_PIN, Y_DIR_PIN, Y_STEP_PIN, Y_CHIP_SELECT);
//static TMC2130Stepper TMC2130_Z = TMC2130Stepper(Z_ENABLE_PIN, Z_DIR_PIN, Z_STEP_PIN, Z_CHIP_SELECT);
//static TMC2130Stepper TMC2130_E = TMC2130Stepper(E_ENABLE_PIN, E_DIR_PIN, E_STEP_PIN, E_CHIP_SELECT);

//static TMC2130Stepper TMC2130X = TMC2130Stepper(X_ENABLE_PIN, X_DIR_PIN, X_STEP_PIN, X_CHIP_SELECT);
//static TMC2130Stepper TMC2130Y = TMC2130Stepper(Y_ENABLE_PIN, Y_DIR_PIN, Y_STEP_PIN, Y_CHIP_SELECT);
//static TMC2130Stepper TMC2130Z = TMC2130Stepper(Z_ENABLE_PIN, Z_DIR_PIN, Z_STEP_PIN, Z_CHIP_SELECT);
//static TMC2130Stepper TMC2130E = TMC2130Stepper(E_ENABLE_PIN, E_DIR_PIN, E_STEP_PIN, E_CHIP_SELECT);

#endif /* TMC2130_H_ */



/*

#if defined(FARMDUINO_EXP_V20)
  #include <TMC2130Stepper.h>
  //#include "TMC2130Stepper.h"
#endif

#ifndef TMC2130Holder_H_
#define TMC2130Holder_H_

class TMC2130Holder
{
public:
  TMC2130Holder();
  TMC2130Holder(TMC2130Holder const &);
  void operator=(TMC2130Holder const &);

  static TMC2130Holder *getInstance();

  void loadDrivers();

  TMC2130Stepper* TMC2130X();
  TMC2130Stepper* TMC2130Y();
  TMC2130Stepper* TMC2130Z();
  TMC2130Stepper* TMC2130E();

private:

  static TMC2130Stepper *tmcTMC2130X;
  static TMC2130Stepper *tmcTMC2130Y;
  static TMC2130Stepper *tmcTMC2130Z;
  static TMC2130Stepper *tmcTMC2130E;

};

#endif 
*/

/* TMC2130Holder_H_ */
