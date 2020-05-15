
/*
 * F11Handler.cpp
 *
 *  Created on: 2014/07/21
 *      Author: MattLech
 */

#include "F11Handler.h"

static F11Handler *instance;

F11Handler *F11Handler::getInstance()
{
  if (!instance)
  {
    instance = new F11Handler();
  };
  return instance;
};

F11Handler::F11Handler()
{
}

int F11Handler::execute(Command *command)
{

  if (LOGGING)
  {
    Serial.print("R99 HOME X\r\n");
  }

  int homeIsUp = ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_X);
  int stepsPerMM = ParameterList::getInstance()->getValue(MOVEMENT_STEP_PER_MM_X);
  int missedStepsMax = ParameterList::getInstance()->getValue(ENCODER_MISSED_STEPS_MAX_X);

  #if defined(FARMDUINO_EXP_V20)
    missedStepsMax += ParameterList::getInstance()->getValue(ENCODER_MISSED_STEPS_DECAY_X);
  #endif

  if (stepsPerMM <= 0)
  {
    missedStepsMax = 0;
    stepsPerMM = 1;
  }

  int A = 10; // move away coordinates
  int execution;
  bool emergencyStop;

  bool homingComplete = false;

  if (homeIsUp == 1)
  {
    A = -A;
  }

  int stepNr;

  long X = CurrentState::getInstance()->getX();
  long Y = CurrentState::getInstance()->getY();
  long Z = CurrentState::getInstance()->getZ();

  bool firstMove = true;
  int goodConsecutiveHomings = 0;

  // After the first home, keep moving away and home back
  // until there is no deviation in positions
  while (!homingComplete)
  {
    if (firstMove)
    {
      firstMove = false;

      // Move to home position
      Movement::getInstance()->moveToCoords(0, Y, Z, 0, 0, 0, false, false, false);
      //execution = CurrentState::getInstance()->getLastError();
      execution = 0;
      emergencyStop = CurrentState::getInstance()->isEmergencyStop();
      if (emergencyStop || execution != 0) { homingComplete = true; }
    }

    // Move away from the home position
    Movement::getInstance()->moveToCoords(A, Y, Z, 0, 0, 0, false, false, false);
    //execution = CurrentState::getInstance()->getLastError();
    execution = 0;
    emergencyStop = CurrentState::getInstance()->isEmergencyStop();
    if (emergencyStop || execution != 0) { break; }

    // Home again
    Movement::getInstance()->moveToCoords(0, Y, Z, 0, 0, 0, true, false, false);
    //execution = CurrentState::getInstance()->getLastError();
    execution = 0;
    emergencyStop = CurrentState::getInstance()->isEmergencyStop();
    if (emergencyStop || execution != 0) { break; }

    Serial.print("R99 homing displaced");
    Serial.print(" X ");
    Serial.print(CurrentState::getInstance()->getHomeMissedStepsX());
    Serial.print(" / ");
    Serial.print(CurrentState::getInstance()->getHomeMissedStepsXscaled());
    Serial.print(" Y ");
    Serial.print(CurrentState::getInstance()->getHomeMissedStepsY());
    Serial.print(" / ");
    Serial.print(CurrentState::getInstance()->getHomeMissedStepsYscaled());
    Serial.print(" Z ");
    Serial.print(CurrentState::getInstance()->getHomeMissedStepsZ());
    Serial.print(" / ");
    Serial.print(CurrentState::getInstance()->getHomeMissedStepsZscaled());
    Serial.print("\r\n");

    // Home position cannot drift more than 5 milimeter otherwise no valid home pos
    if (CurrentState::getInstance()->getHomeMissedStepsXscaled() < (20 + (missedStepsMax * 3) / stepsPerMM))
    {
      goodConsecutiveHomings++;
      if (goodConsecutiveHomings >= 3)
      {
        homingComplete = true;
        CurrentState::getInstance()->setX(0);
      }
    }
    else
    {
      delay(500);
      goodConsecutiveHomings = 0;
    }
  }

  if (LOGGING)
  {
    CurrentState::getInstance()->print();
  }

  return 0;
}
