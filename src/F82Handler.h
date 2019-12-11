/*
 * F82Handler.h
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#ifndef F82HANDLER_H_
#define F82HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "Movement.h"

class F82Handler : public GCodeHandler
{
public:
  static F82Handler *getInstance();
  int execute(Command *);

private:
  F82Handler();
  F82Handler(F82Handler const &);
  void operator=(F82Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* F82HANDLER_H_ */
