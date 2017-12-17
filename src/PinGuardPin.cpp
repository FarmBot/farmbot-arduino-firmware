
#include "PinGuardPin.h"
#include "ParameterList.h"
#include "PinControl.h"

PinGuardPin::PinGuardPin()
{
  pinNr = 0;
}

// Set the initial settings for what pin to check
void PinGuardPin::loadPinConfig(int pinNrParamNr, int activeStateParamNr, int timeOutParamNr)
{

  pinNr = ParameterList::getInstance()->getValue(pinNrParamNr);
  activeState = (ParameterList::getInstance()->getValue(activeStateParamNr) == 1);
  timeOut = ParameterList::getInstance()->getValue(timeOutParamNr);

  timeOutCount = 0;
}

// Check each second if the time out is lapsed or the value has changed
void PinGuardPin::processTick()
{

  if (pinNr == 0)
  {
    return;
  }

  currentStatePin = digitalRead(pinNr);

  if (currentStatePin != activeState)
  {
    timeOutCount = 0;
  }
  else
  {
    timeOutCount++;
  }

  if (timeOutCount >= timeOut)
  {
    digitalWrite(pinNr, !activeState);
    PinControl::getInstance()->readValue(pinNr, 0);
  }
}
