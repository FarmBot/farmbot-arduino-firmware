/*
 * GCodeHandler.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef GCODEHANDLER_H_
#define GCODEHANDLER_H_
#include "Command.h"

class GCodeHandler
{
public:
  GCodeHandler();
  virtual ~GCodeHandler();
  virtual int execute(Command *);
};

#endif /* GCODEHANDLER_H_ */
