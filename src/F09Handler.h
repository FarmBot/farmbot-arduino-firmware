/*
 * F09Handler.h
 *
 *  Created on: 2017/04/26
 *      Author: Tim Evers
 */

#ifndef F09HANDLER_H_
#define F09HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"

class F09Handler : public GCodeHandler
{
public:
  static F09Handler *getInstance();
  int execute(Command *);

private:
  F09Handler();
  F09Handler(F09Handler const &);
  void operator=(F09Handler const &);
};

#endif /* F09HANDLER_H_ */
