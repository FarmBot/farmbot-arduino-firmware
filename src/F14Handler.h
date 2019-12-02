
/*
 * F14Handler.h
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#ifndef F14HANDLER_H_
#define F14HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "Movement.h"

class F14Handler : public GCodeHandler
{
public:
  static F14Handler *getInstance();
  int execute(Command *);

private:
  F14Handler();
  F14Handler(F14Handler const &);
  void operator=(F14Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* F14HANDLER_H_ */
