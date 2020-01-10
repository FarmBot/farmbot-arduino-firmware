/*
 * TMC2130.h
 *
 *  Created on: 03 nov 2019
 *      Author: Tim Evers
 */


#ifndef TMC2130_H_
#define TMC2130_H_

#include <SPI.h>
#include "Trinamic\Trinamic_TMC2130.h"
#include "pins.h"

#include "TMC2130_Basics.h"

//static Trinamic_TMC2130 TMC2130X(X_CHIP_SELECT);
//static Trinamic_TMC2130 TMC2130Y(Y_CHIP_SELECT);
//static Trinamic_TMC2130 TMC2130Z(Z_CHIP_SELECT);
//static Trinamic_TMC2130 TMC2130E(E_CHIP_SELECT);

static TMC2130_Basics TMC2130X(X_CHIP_SELECT);
static TMC2130_Basics TMC2130Y(Y_CHIP_SELECT);
static TMC2130_Basics TMC2130Z(Z_CHIP_SELECT);
static TMC2130_Basics TMC2130E(E_CHIP_SELECT);

//void loadTMC2130ParametersMotor(Trinamic_TMC2130 *myStepper, int microsteps, int current, int sensitivity);
void loadTMC2130ParametersMotor(TMC2130_Basics *tb, int microsteps, int current, int sensitivity);


#endif /* TMC2130_H_ */