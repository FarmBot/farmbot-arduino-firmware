#include "StatusList.h"

static StatusList *instanceParam;
long statusValues[150];

StatusList *StatusList::getInstance()
{
  if (!instanceParam)
  {
    instanceParam = new StatusList();
  };
  return instanceParam;
}

StatusList::StatusList()
{

  statusValues[STATUS_GENERAL] = STATUS_GENERAL_DEFAULT;

  //paramValues[MOVEMENT_MAX_SPD_X]          = MOVEMENT_MAX_SPD_X_DEFAULT;
  //paramValues[MOVEMENT_MAX_SPD_Y]          = MOVEMENT_MAX_SPD_Y_DEFAULT;
  //paramValues[MOVEMENT_MAX_SPD_Z]          = MOVEMENT_MAX_SPD_Z_DEFAULT;
}

int StatusList::readValue(int id)
{

  long value = statusValues[id];

  Serial.print("R31");
  Serial.print(" ");
  Serial.print("P");
  Serial.print(id);
  Serial.print(" ");
  Serial.print("V");
  Serial.print(value);
  //Serial.print("\r\n");
  CurrentState::getInstance()->printQAndNewLine();

  return 0;
}

long StatusList::getValue(int id)
{

  /*
	Serial.print("R99");
	Serial.print(" ");
	Serial.print("getValue");
	Serial.print(" id ");
	Serial.print(id);
	Serial.print(" value");
	Serial.print(paramValues[id]);
	Serial.print("\r\n");
*/

  return statusValues[id];
}

int StatusList::setValue(int id, long value)
{

  statusValues[id] = value;

  return 0;
}
