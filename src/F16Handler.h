
/*
 * F16Handler.h
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#ifndef F16HANDLER_H_
#define F16HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "Movement.h"

class F16Handler : public GCodeHandler
{
public:
  static F16Handler *getInstance();
  int execute(Command *);

private:
  F16Handler();
  F16Handler(F16Handler const &);
  void operator=(F16Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* F16HANDLER_H_ */
