#ifndef STATUSLIST_H_
#define STATUSLIST_H_

#include "Arduino.h"
#include "Config.h"
#include "CurrentState.h"

//#define NULL 0

enum StatusListEnum
{
  STATUS_GENERAL = 0,

  //MOVEMENT_MAX_SPD_X		= 71,
  //MOVEMENT_MAX_SPD_Y		= 72,
  //MOVEMENT_MAX_SPD_Z		= 73

};

/*
#define NULL 0
*/

class StatusList
{
  StatusListEnum statusListEnum;

public:
  static StatusList *getInstance();
  int writeValue(int id, long value);
  int readValue(int id);
  long getValue(int id);
  int setValue(int id, long value);

private:
  StatusList();
  StatusList(StatusList const &);
  void operator=(StatusList const &);
};

#endif /* STATUSLIST_H_ */
