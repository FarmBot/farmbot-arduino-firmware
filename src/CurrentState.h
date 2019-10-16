/*
 * CurrentState.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef CURRENTSTATE_H_
#define CURRENTSTATE_H_
#include "Arduino.h"
#include "pins.h"

enum ErrorListEnum
{
  ERR_NO_ERROR = 0,
  ERR_EMERGENCY_STOP = 1,
  ERR_TIMEOUT = 2,
  ERR_STALL_DETECTED = 3,

  ERR_INVALID_COMMAND = 14,
  ERR_PARAMS_NOT_OK = 15,

};

class CurrentState
{
public:
  static CurrentState *getInstance();

  long getX();
  long getY();
  long getZ();
  long *getPoint();
  void setX(long);
  void setY(long);
  void setZ(long);

  int getLastError();
  void setLastError(int error);

  void setEndStopState(unsigned int, unsigned int, bool);
  void printPosition();
  String getPosition();
  void storeEndStops();
  void printEndStops();
  void print();
  void printBool(bool);

  void setQ(long);
  void resetQ();
  void printQAndNewLine();
  String getQAndNewLine();

  void setEmergencyStop();
  void resetEmergencyStop();
  bool isEmergencyStop();

  void setStepsPerMm(long stepsX, long stepsY, long stepsZ);

private:
  CurrentState();
  CurrentState(CurrentState const &);
  void operator=(CurrentState const &);

  long stepsPerMmX;
  long stepsPerMmY;
  long stepsPerMmZ;

  int errorCode = 0;

  bool emergencyStop = false;
};

#endif /* CURRENTSTATE_H_ */
