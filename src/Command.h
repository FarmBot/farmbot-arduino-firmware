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
  F14 = 114,
  F15 = 115,
  F16 = 116,
  F20 = 120,
  F21 = 121,
  F22 = 122,
  F31 = 131,
  F32 = 132,
  F41 = 141,
  F42 = 142,
  F43 = 143,
  F44 = 144,
  F61 = 161,
  F81 = 181,
  F82 = 182,
  F83 = 183
};

#define NULL 0

class Command {
	CommandCodeEnum codeEnum;
public:
//	Command(String);
	Command(char * commandChar);
	void print();
	CommandCodeEnum getCodeEnum();
	double getX();
	double getY();
	double getZ();
	double getS();
	long getP();
	long getV();
	long getA();
	long getB();
	long getC();
	long getW();
	long getT();
	long getE();
	long getM();
	long getQ();

	void printQAndNewLine();
private:
	CommandCodeEnum getGCodeEnum(char* code);
	void getParameter(char* charPointer);
};

#endif /* COMMAND_H_ */
