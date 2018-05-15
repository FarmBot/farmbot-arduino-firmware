
/*
 * PinGuardPin.h
 *
 *  Created on: 2016-04-02
 *      Author: Tim Evers
 */

#ifndef PINGUARDPIN_H_
#define PINGUARDPIN_H_

#include "Arduino.h"
#include "pins.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>
//#include "ParameterList.h"

class PinGuardPin
{
public:
  PinGuardPin();

  void processTick();
  void loadPinConfig(int pinNrParamNr, int activeStateParamNr, int timeOutParamNr);

private:
  //PinControlPin(PinControlPin const&);
  ///void operator=(PinControlPin const&);

  int pinNr;
  long timeOut;
  long timeOutCount;
  bool activeState;
  bool currentStatePin;
};

#endif /* PINGUARDPIN_H_ */
