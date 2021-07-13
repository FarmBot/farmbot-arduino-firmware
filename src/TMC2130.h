/*
 * TMC2130.h
 *
 *  Created on: 03 nov 2019
 *      Author: Tim Evers
 */


#ifndef TMC2130_H_
#define TMC2130_H_

#include <SPI.h>

#include "pins.h"
#include "Board.h"

#if defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_V30)
  #include "TMC2130_Basics.h"

  static TMC2130_Basics TMC2130X(X_CHIP_SELECT, 1);
  static TMC2130_Basics TMC2130Y(Y_CHIP_SELECT, 2);
  static TMC2130_Basics TMC2130Z(Z_CHIP_SELECT, 3);
  static TMC2130_Basics TMC2130E(E_CHIP_SELECT, 4);

  void loadTMC2130ParametersMotor(TMC2130_Basics *tb, int microsteps, int current, int sensitivity, bool stealth);
#endif

#endif /* TMC2130_H_ */