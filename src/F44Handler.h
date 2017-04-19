/*
 * F44Handler.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef F44HANDLER_H_
#define F44HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "PinControl.h"

class F44Handler : public GCodeHandler
{
public:
  static F44Handler *getInstance();
  int execute(Command *);

private:
  F44Handler();
  F44Handler(F44Handler const &);
  void operator=(F44Handler const &);
};

#endif /* F44HANDLER_H_ */
