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
  bool isMovement = false;

  bool emergencyStop = false;
  emergencyStop = CurrentState::getInstance()->isEmergencyStop();

  bool movementAbort = false;
  movementAbort = CurrentState::getInstance()->isMovementAbort();

  int attempt = 0;
  int maximumAttempts = ParameterList::getInstance()->getValue(PARAM_MOV_NR_RETRY);

  long Q = command->getQ();
  CurrentState::getInstance()->setQ(Q);

  if
  (
    command->getCodeEnum() == G00 ||
    command->getCodeEnum() == G01 ||
    command->getCodeEnum() == F11 ||
    command->getCodeEnum() == F12 ||
    command->getCodeEnum() == F13 ||
    command->getCodeEnum() == F14 ||
    command->getCodeEnum() == F15 ||
    command->getCodeEnum() == F16
  )
  {
    isMovement = true;
  }



  //Only allow reset of emergency stop when emergency stop is engaged

  if (emergencyStop)
  {
    if (!(
      command->getCodeEnum() == F09 ||
      command->getCodeEnum() == F20 ||
      command->getCodeEnum() == F21 ||
      command->getCodeEnum() == F22 ||
      command->getCodeEnum() == F81 ||
      command->getCodeEnum() == F82 ||
      command->getCodeEnum() == F83 ))
    {

    Serial.print(COMM_REPORT_EMERGENCY_STOP);
    CurrentState::getInstance()->printQAndNewLine();
    return -1;
    }
  }

  // Do not execute the command when the config complete parameter is not
  // set by the raspberry pi and it's asked to do a move command

  if (ParameterList::getInstance()->getValue(PARAM_CONFIG_OK) != 1)
  {
    if (isMovement)
    {
      Serial.print(COMM_REPORT_NO_CONFIG);
      CurrentState::getInstance()->printQAndNewLine();
      CurrentState::getInstance()->setLastError(ERR_PARAMS_NOT_OK);
      return -1;
    }
  }

  // Return error when no command or invalid command is found

  if (command == NULL)
  {

    Serial.print(COMM_REPORT_BAD_CMD);
    CurrentState::getInstance()->printQAndNewLine();
    CurrentState::getInstance()->setLastError(ERR_INVALID_COMMAND);

    if (LOGGING)
    {
      printCommandLog(command);
    }
    return -1;
  }

  if (command->getCodeEnum() == CODE_UNDEFINED)
  {
    Serial.print(COMM_REPORT_BAD_CMD);
    CurrentState::getInstance()->printQAndNewLine();
    CurrentState::getInstance()->setLastError(ERR_INVALID_COMMAND);

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
    Serial.print(COMM_REPORT_BAD_CMD);
    CurrentState::getInstance()->printQAndNewLine();
    CurrentState::getInstance()->setLastError(ERR_INVALID_COMMAND);

    Serial.println("R99 handler == NULL\r\n");
    return -1;
  }

  // Report start of command

  Serial.print(COMM_REPORT_CMD_START);
  CurrentState::getInstance()->printQAndNewLine();

  // Execute command with retry
  CurrentState::getInstance()->setLastError(0);
  while (attempt < 1 || (!emergencyStop && !movementAbort && attempt < maximumAttempts && execution != 0 && execution != 2))
  // error 2 is timeout error: stop movement retries
  {

    if (LOGGING || debugMessages)
    {
      Serial.print("R99 attempt ");
      Serial.print(attempt);
      Serial.print(" from ");
      Serial.print(maximumAttempts);
      Serial.print("\r\n");
    }

    attempt++;
    if (attempt > 1)
    {
      Serial.print(COMM_REPORT_CMD_RETRY);
      CurrentState::getInstance()->printQAndNewLine();
    }
    
    handler->execute(command);
    execution = CurrentState::getInstance()->getLastError();
    emergencyStop = CurrentState::getInstance()->isEmergencyStop();
    movementAbort = CurrentState::getInstance()->isMovementAbort();

    if (LOGGING || debugMessages)
    {
      Serial.print("R99 execution ");
      Serial.print(execution);
      Serial.print("\r\n");
    }
  }

  // Clean serial buffer
  while (Serial.available() > 0)
  {
    Serial.read();
  }

  // if movemement failed after retry
  // and parameter for emergency stop is set
  // set the emergency stop

  if (execution != 0)
  {
    if (isMovement)
    {
      if (ParameterList::getInstance()->getValue(PARAM_E_STOP_ON_MOV_ERR) == 1)
      {
        CurrentState::getInstance()->setEmergencyStop();
      }
    }
  }

  // Report back result of execution
  if (execution == 0)
  {
    Serial.print(COMM_REPORT_CMD_DONE);
    CurrentState::getInstance()->printQAndNewLine();
  }
  else
  {
    Serial.print(COMM_REPORT_CMD_ERROR);
    Serial.print(" V");
    Serial.print(CurrentState::getInstance()->getLastError());
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

  // Edited by Finn:
  if (codeEnum == F01)
  {
    handler = F01Handler::getInstance();
  }

  // Edited by Finn:
  if (codeEnum == F02)
  {
    handler = F02Handler::getInstance();
  }

  if (codeEnum == F09)
  {
    handler = F09Handler::getInstance();
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
