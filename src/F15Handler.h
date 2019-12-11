
/*
 * F15Handler.h
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#ifndef F15HANDLER_H_
#define F15HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "Movement.h"

class F15Handler : public GCodeHandler
{
public:
  static F15Handler *getInstance();
  int execute(Command *);

private:
  F15Handler();
  F15Handler(F15Handler const &);
  void operator=(F15Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* F15HANDLER_H_ */
