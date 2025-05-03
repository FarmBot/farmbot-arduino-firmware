#include "Command.h"

const char axisCodes[3] = {'X', 'Y', 'Z'};
const char axisSpeedCodes[3] = {'A', 'B', 'C'};
const char speedCode = 'S';
const char parameterIdCode = 'P';
const char parameterValueCode = 'V';
const char parameterValue2Code = 'W';
const char elementCode = 'E';
const char timeCode = 'T';
const char modeCode = 'M';
const char numberCode = 'N';    // Edited by Finn
const char msgQueueCode = 'Q';

CommandCodeEnum commandCodeEnum = CODE_UNDEFINED;

Command::Command(char *commandChar)
{

  char *charBuf = commandChar;
  char *charPointer;
  bool invalidCommand = false;

  charPointer = strtok(charBuf, " \n\r\0");

  if (charPointer[0] == 'G' || charPointer[0] == 'F')
  {
    commandCodeEnum = getGCodeEnum(charPointer);
  }
  else
  {
    invalidCommand = true;
    commandCodeEnum = CODE_UNDEFINED;
    return;
  }

  while (charPointer != NULL)
  {
    getParameter(charPointer);

    charPointer = strtok(NULL, " \n\r");
  }
}

CommandCodeEnum Command::getGCodeEnum(char *code)
{

  if (strcmp(code, "G0") == 0 || strcmp(code, "G00") == 0)
  {
    return G00;
  }
  if (strcmp(code, "G1") == 0 || strcmp(code, "G01") == 0)
  {
    return G01;
  }
    if (strcmp(code, "G28") == 0)
  {
    return G28;
  }

  //if (strcmp(code, "F3") == 0 || strcmp(code, "F03") == 0) {
  //	return F03;
  //}

  // Finn: F01 
  if (strcmp(code, "F01") == 0 || strcmp(code, "F1") == 0)
  {
    return F01;
  }

  // Finn: F02 
  if (strcmp(code, "F02") == 0 || strcmp(code, "F2") == 0)
  {
    return F02;
  }

  if (strcmp(code, "F09") == 0 || strcmp(code, "F9") == 0)
  {
    return F09;
  }

  if (strcmp(code, "F11") == 0)
  {
    return F11;
  }
  if (strcmp(code, "F12") == 0)
  {
    return F12;
  }
  if (strcmp(code, "F13") == 0)
  {
    return F13;
  }
  if (strcmp(code, "F14") == 0)
  {
    return F14;
  }
  if (strcmp(code, "F15") == 0)
  {
    return F15;
  }
  if (strcmp(code, "F16") == 0)
  {
    return F16;
  }

  if (strcmp(code, "F20") == 0)
  {
    return F20;
  }
  if (strcmp(code, "F21") == 0)
  {
    return F21;
  }
  if (strcmp(code, "F22") == 0)
  {
    return F22;
  }

  if (strcmp(code, "F31") == 0)
  {
    return F31;
  }
  if (strcmp(code, "F32") == 0)
  {
    return F32;
  }

  if (strcmp(code, "F41") == 0)
  {
    return F41;
  }
  if (strcmp(code, "F42") == 0)
  {
    return F42;
  }
  if (strcmp(code, "F43") == 0)
  {
    return F43;
  }
  if (strcmp(code, "F44") == 0)
  {
    return F44;
  }

  if (strcmp(code, "F61") == 0)
  {
    return F61;
  }

  if (strcmp(code, "F81") == 0)
  {
    return F81;
  }
  if (strcmp(code, "F82") == 0)
  {
    return F82;
  }
  if (strcmp(code, "F83") == 0)
  {
    return F83;
  }
  if (strcmp(code, "F84") == 0)
  {
    return F84;
  }

  return CODE_UNDEFINED;
}

double minusNotAllowed(double value)
{
  if (value < 0)
  {
    return 0;
  }
  return value;
}

void Command::getParameter(char *charPointer)
{

  //msgQueue          =  24;

  if (charPointer[0] == axisCodes[0])
  {
    axisValue[0] = atof(charPointer + 1);
    //msgQueue	= 77;
  }

  if (charPointer[0] == axisCodes[1])
  {
    axisValue[1] = atof(charPointer + 1);
  }

  if (charPointer[0] == axisCodes[2])
  {
    axisValue[2] = atof(charPointer + 1);
  }

  if (charPointer[0] == axisSpeedCodes[0])
  {
    axisSpeedValue[0] = atof(charPointer + 1);
  }

  if (charPointer[0] == axisSpeedCodes[1])
  {
    axisSpeedValue[1] = atof(charPointer + 1);
  }

  if (charPointer[0] == axisSpeedCodes[2])
  {
    axisSpeedValue[2] = atof(charPointer + 1);
  }

  if (charPointer[0] == speedCode)
  {
    speedValue = atof(charPointer + 1);
  }

  if (charPointer[0] == parameterIdCode)
  {
    parameterId = atof(charPointer + 1);
  }

  if (charPointer[0] == parameterValueCode)
  {
    parameterValue = atof(charPointer + 1);
  }

  if (charPointer[0] == parameterValue2Code)
  {
    parameterValue2 = atof(charPointer + 1);
  }

  if (charPointer[0] == elementCode)
  {
    element = atof(charPointer + 1);
  }

  if (charPointer[0] == timeCode)
  {
    time = atof(charPointer + 1);
  }

  // Edited by Finn
  if (charPointer[0] == numberCode) {
    number = atof(charPointer + 1);
    // Serial.print("Set number to: ");
    // Serial.println(number);
  }

  if (charPointer[0] == modeCode)
  {
    mode = atof(charPointer + 1);
  }

  if (charPointer[0] == msgQueueCode)
  {
    msgQueue = atof(charPointer + 1);
    //msgQueue   =  5;
  }
}

void Command::print()
{
  Serial.print("R99 Command with code: ");
  Serial.print(commandCodeEnum);
  Serial.print(", X: ");
  Serial.print(axisValue[0]);
  Serial.print(", Y: ");
  Serial.print(axisValue[1]);
  Serial.print(", Z: ");
  Serial.print(axisValue[2]);
  Serial.print(", S: ");
  Serial.print(speedValue);
  Serial.print(", P: ");
  Serial.print(parameterId);
  Serial.print(", V: ");
  Serial.print(parameterValue);

  Serial.print(", W: ");
  Serial.print(parameterValue2);
  Serial.print(", T: ");
  Serial.print(time);
  Serial.print(", E: ");
  Serial.print(element);
  Serial.print(", M: ");
  Serial.print(mode);
  Serial.print(", N: ");
  Serial.print(number);

  Serial.print(", A: ");
  Serial.print(axisSpeedValue[0]);
  Serial.print(", B: ");
  Serial.print(axisSpeedValue[1]);
  Serial.print(", C: ");
  Serial.print(axisSpeedValue[2]);

  Serial.print(", Q: ");
  Serial.print(msgQueue);

  Serial.print("\r\n");
}

CommandCodeEnum Command::getCodeEnum()
{
  return commandCodeEnum;
}

double Command::getX()
{
  return axisValue[0];
}

double Command::getY()
{
  return axisValue[1];
}

double Command::getZ()
{
  return axisValue[2];
}

long Command::getA()
{
  return axisSpeedValue[0];
}

long Command::getB()
{
  return axisSpeedValue[1];
}

long Command::getC()
{
  return axisSpeedValue[2];
}

long Command::getP()
{
  return parameterId;
}

long Command::getV()
{
  return parameterValue;
}

long Command::getW()
{
  return parameterValue2;
}

long Command::getT()
{
  return time;
}

long Command::getE()
{
  return element;
}

long Command::getM()
{
  return mode;
}

long Command::getN()  // Edited by Finn
{
  return number;
}

long Command::getQ()
{
  //msgQueue = 9876;
  return msgQueue;
}
