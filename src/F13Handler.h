/*
 * F13Handler.h
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#ifndef F13HANDLER_H_
#define F13HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "Movement.h"

class F13Handler : public GCodeHandler
{
public:
  static F13Handler *getInstance();
  int execute(Command *);

private:
  F13Handler();
  F13Handler(F13Handler const &);
  void operator=(F13Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* F13HANDLER_H_ */
