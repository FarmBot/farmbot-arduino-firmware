/*
 * F43Handler.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef F43HANDLER_H_
#define F43HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "PinControl.h"

class F43Handler : public GCodeHandler
{
public:
  static F43Handler *getInstance();
  int execute(Command *);

private:
  F43Handler();
  F43Handler(F43Handler const &);
  void operator=(F43Handler const &);
};

#endif /* F43HANDLER_H_ */
