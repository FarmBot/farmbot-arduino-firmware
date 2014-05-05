#include "Arduino.h"
#include "string.h"
enum CommandCodeEnum
{
  CODE_UNDEFINED = -1,
  G0 = 0,
  G1,
  G2,
  G3,
  G4
};

#define NULL 0

class Command {
	CommandCodeEnum codeEnum;
public:
	Command(String);
	void print();
private:
	CommandCodeEnum getGCodeEnum(char* code);
	void getParameter(char* charPointer);
};
