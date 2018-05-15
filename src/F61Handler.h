/*
 * F61Handler.h
 *
 *  Created on: 2014-11-04
 *      Author: TimEvers
 */

#ifndef F4HANDLER_H_
#define F4HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "ServoControl.h"

class F61Handler : public GCodeHandler
{
public:
  static F61Handler *getInstance();
  int execute(Command *);

private:
  F61Handler();
  F61Handler(F61Handler const &);
  void operator=(F61Handler const &);
};

#endif /* F61HANDLER_H_ */
