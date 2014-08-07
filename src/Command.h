#ifndef COMMAND_H_
#define COMMAND_H_

#include "Arduino.h"
//#include "string.h"

enum CommandCodeEnum
{
  CODE_UNDEFINED = -1,
  G00 =   0,
  G01 =   1,
  G28 =  28,
  F01 = 101,
  F02 = 102,
  F03 = 103,
  F11 = 111,
  F12 = 112,
  F13 = 113,
  F20 = 120,
  F21 = 121,
  F22 = 122,
  F81 = 181,
  F82 = 182,
  F83 = 183
};

#define NULL 0

class Command {
	CommandCodeEnum codeEnum;
public:
	Command(String);
	void print();
	CommandCodeEnum getCodeEnum();
	double getX();
	double getY();
	double getZ();
	double getS();
	double getP();
	double getV();
	long getA();
	long getB();
	long getC();
private:
	CommandCodeEnum getGCodeEnum(char* code);
	void getParameter(char* charPointer);
};

#endif /* COMMAND_H_ */
