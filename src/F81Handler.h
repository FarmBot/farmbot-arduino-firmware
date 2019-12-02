/*
 * F81Handler.h
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#ifndef F81HANDLER_H_
#define F81HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "Movement.h"
#include "PinControl.h"

class F81Handler : public GCodeHandler
{
public:
  static F81Handler *getInstance();
  int execute(Command *);

private:
  F81Handler();
  F81Handler(F81Handler const &);
  void operator=(F81Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* F81HANDLER_H_ */
