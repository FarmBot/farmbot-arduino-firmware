/*
 * GCodeProcessor.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef GCODEPROCESSOR_H_
#define GCODEPROCESSOR_H_
#include "GCodeHandler.h"
#include "G00Handler.h"
#include "Command.h"
#include "Config.h"

class GCodeProcessor {
public:
	GCodeProcessor();
	virtual ~GCodeProcessor();
	int execute(Command* command);
protected:
	GCodeHandler* getGCodeHandler(CommandCodeEnum);
private:
	void printCommandLog(Command*);
};

#endif /* GCODEPROCESSOR_H_ */
