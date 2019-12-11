/*
 * F12Handler.h
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#ifndef F12HANDLER_H_
#define F12HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "Movement.h"

class F12Handler : public GCodeHandler
{
public:
  static F12Handler *getInstance();
  int execute(Command *);

private:
  F12Handler();
  F12Handler(F12Handler const &);
  void operator=(F12Handler const &);
  long adjustStepAmount(long);
  long getNumberOfSteps(double, double);
};

#endif /* F12HANDLER_H_ */
