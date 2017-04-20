/*
 * GCodeProcessor.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 *      Author: Tim Evers
 */

#include "GCodeProcessor.h"
#include "CurrentState.h"

GCodeProcessor::GCodeProcessor()
{
}

GCodeProcessor::~GCodeProcessor()
{
}

void GCodeProcessor::printCommandLog(Command *command)
{
  Serial.print("command == NULL: ");
  Serial.println("\r\n");
}

int GCodeProcessor::execute(Command *command)
{

  int execution = 0;

  long Q = command->getQ();
  CurrentState::getInstance()->setQ(Q);

  // Do not execute the command when the config complete parameter is not
  // set by the raspberry pi and it's asked to do a move command

  // Tim 2017-04-15 Disable until the raspberry code is ready
  /*
	if (ParameterList::getInstance()->getValue(PARAM_CONFIG_OK) != 1) {
		if (	command->getCodeEnum() == G00 ||
			command->getCodeEnum() == G01 ||
			command->getCodeEnum() == F11 ||
			command->getCodeEnum() == F12 ||
			command->getCodeEnum() == F13 ||
			command->getCodeEnum() == F14 ||
			command->getCodeEnum() == F15 ||
			command->getCodeEnum() == F16 ) {

        		Serial.print(COMM_REPORT_NO_CONFIG);
			CurrentState::getInstance()->printQAndNewLine();
			return -1;
		}
        }
	*/

  // Return error when no command or invalid command is found

  if (command == NULL)
  {
    if (LOGGING)
    {
      printCommandLog(command);
    }
    return -1;
  }

  if (command->getCodeEnum() == CODE_UNDEFINED)
  {
    if (LOGGING)
    {
      printCommandLog(command);
    }
    return -1;
  }

  // Get the right handler for this command

  GCodeHandler *handler = getGCodeHandler(command->getCodeEnum());

  if (handler == NULL)
  {
    Serial.println("R99 handler == NULL\r\n");
    return -1;
  }

  // Execute te command, report start and end

  Serial.print(COMM_REPORT_CMD_START);
  CurrentState::getInstance()->printQAndNewLine();

  execution = handler->execute(command);
  if (execution == 0)
  {
    Serial.print(COMM_REPORT_CMD_DONE);
    CurrentState::getInstance()->printQAndNewLine();
  }
  else
  {
    Serial.print(COMM_REPORT_CMD_ERROR);
    CurrentState::getInstance()->printQAndNewLine();
  }

  CurrentState::getInstance()->resetQ();
  return execution;
};

GCodeHandler *GCodeProcessor::getGCodeHandler(CommandCodeEnum codeEnum)
{

  GCodeHandler *handler = NULL;

  // These are if statements so they can be disabled as test
  // Usefull when running into memory issues again

  if (codeEnum == G00)
  {
    handler = G00Handler::getInstance();
  }

  if (codeEnum == G28)
  {
    handler = G28Handler::getInstance();
  }

  if (codeEnum == F11)
  {
    handler = F11Handler::getInstance();
  }
  if (codeEnum == F12)
  {
    handler = F12Handler::getInstance();
  }
  if (codeEnum == F13)
  {
    handler = F13Handler::getInstance();
  }

  if (codeEnum == F14)
  {
    handler = F14Handler::getInstance();
  }
  if (codeEnum == F15)
  {
    handler = F15Handler::getInstance();
  }
  if (codeEnum == F16)
  {
    handler = F16Handler::getInstance();
  }

  if (codeEnum == F20)
  {
    handler = F20Handler::getInstance();
  }
  if (codeEnum == F21)
  {
    handler = F21Handler::getInstance();
  }
  if (codeEnum == F22)
  {
    handler = F22Handler::getInstance();
  }

  //	if (codeEnum == F31) {handler = F31Handler::getInstance();}
  //	if (codeEnum == F32) {handler = F32Handler::getInstance();}

  if (codeEnum == F41)
  {
    handler = F41Handler::getInstance();
  }
  if (codeEnum == F42)
  {
    handler = F42Handler::getInstance();
  }
  if (codeEnum == F43)
  {
    handler = F43Handler::getInstance();
  }
  if (codeEnum == F44)
  {
    handler = F44Handler::getInstance();
  }

  if (codeEnum == F61)
  {
    handler = F61Handler::getInstance();
  }

  if (codeEnum == F81)
  {
    handler = F81Handler::getInstance();
  }
  if (codeEnum == F82)
  {
    handler = F82Handler::getInstance();
  }
  if (codeEnum == F83)
  {
    handler = F83Handler::getInstance();
  }
  if (codeEnum == F84)
  {
    handler = F84Handler::getInstance();
  }

  return handler;
}
