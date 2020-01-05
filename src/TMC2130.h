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

//static Trinamic_TMC2130 *TMC2130X;
//static Trinamic_TMC2130 *TMC2130Y;
//static Trinamic_TMC2130 *TMC2130Z;
//static Trinamic_TMC2130 *TMC2130E;


static Trinamic_TMC2130 TMC2130X(X_CHIP_SELECT);
static Trinamic_TMC2130 TMC2130Y(Y_CHIP_SELECT);
static Trinamic_TMC2130 TMC2130Z(Z_CHIP_SELECT);
static Trinamic_TMC2130 TMC2130E(E_CHIP_SELECT);

void loadTMC2130ParametersMotor(Trinamic_TMC2130 *myStepper, int microsteps, int current, int sensitivity);

//static TMC2130Stepper *TMC2130X;
//static TMC2130Stepper *TMC2130Y;
//static TMC2130Stepper *TMC2130Z;
//static TMC2130Stepper *TMC2130E;

#endif /* TMC2130_H_ */