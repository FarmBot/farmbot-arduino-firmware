
/*
 * PinGuard.h
 *
 *  Created on: 2016-03-13
 *      Author: Tim Evers
 */

#ifndef PINGUARD_H_
#define PINGUARD_H_

#include "Arduino.h"
#include "pins.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>
#include "PinGuardPin.h"
#include "ParameterList.h"

class PinGuard
{
public:
  static PinGuard *getInstance();

  void loadConfig();
  void checkPins();

private:
  //long pinTimeOut[100];
  //long pinCurrentTime[100];
  //void checkPin;
  //bool pinSafeState[100];

  PinGuardPin pinGuardPin[5];
  //PinGuardPin test;

  PinGuard();
  PinGuard(PinGuard const &);
  void operator=(PinGuard const &);
};

#endif /* PINGUARD_H_ */
