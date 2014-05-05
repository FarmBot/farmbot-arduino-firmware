#include "Arduino.h"
#include "string.h"
enum CommandCodeEnum
{
  G00 = 0,
  G01,
  G02,
  G03,
  G04
};

#define NULL 0

class Command {
	CommandCodeEnum codeEnum;
public:
	Command(String);
};
