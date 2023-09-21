// Do not remove the include below
#include "farmbot_arduino_controller.h"

//#if !(defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_EXP_V22)) || defined(FARMDUINO_V30) || defined(FARMDUINO_V32)
#include "TimerOne.h"
//#endif

bool stepperInit = false;
bool stepperFlip = false;

static char commandEndChar = 0x0A;
char commandChar[INCOMING_CMD_BUF_SIZE + 1];
//String commandString = "";
char incomingChar = 0;
char incomingCommandArray[INCOMING_CMD_BUF_SIZE];
int incomingCommandPointer = 0;

static GCodeProcessor *gCodeProcessor = new GCodeProcessor();

int pinNr = 0;
int pinValue = 0;
int reportingPeriod = 5000;
int reportingPeriodPins = 500;
unsigned long lastAction;
unsigned long lastReportPin;
unsigned long currentTime;
unsigned long cycleCounter = 0;
bool previousEmergencyStop = false;

unsigned long pinGuardLastCheck;
unsigned long pinGuardCurrentTime;

int lastParamChangeNr = 0;
int lastTmcParamChangeNr = 0;

// Blink led routine used for testing
bool blink = false;
void blinkLed()
{
  blink = !blink;
  //digitalWrite(LED_PIN, blink);
  digitalWrite(13, blink);
}

// Interrupt handling for:
//   - movement
//   - encoders
//   - pin guard
//

unsigned long interruptStartTime = 0;
unsigned long interruptStopTime = 0;
unsigned long interruptDuration = 0;
unsigned long interruptDurationMax = 0;

bool interruptBusy = false;
int interruptSecondTimer = 0;

//#if !(defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_EXP_V22)) || defined(FARMDUINO_V30) || defined(FARMDUINO_V32)
void interrupt(void)
{
  if (!debugInterrupt)
  {
    //interruptSecondTimer++;

    if (interruptBusy == false)
    {
      //interruptStartTime = micros();

      interruptBusy = true;
      Movement::getInstance()->handleMovementInterrupt();
      interruptBusy = false;
    }
  }
}
//#endif

#if defined(FARMDUINO_EXP_V20xxx) || defined(FARMDUINO_EXP_V22xxx)
ISR(TIMER2_OVF_vect) {

  if (interruptBusy == false)
  {
    interruptBusy = true;

    Movement::getInstance()->handleMovementInterrupt();
    interruptBusy = false;
  }
}
#endif

void checkPinGuard()
{
  pinGuardCurrentTime = millis();
  if (pinGuardCurrentTime < pinGuardLastCheck)
  {
    pinGuardLastCheck = pinGuardCurrentTime;
  }
  else
  {
    if (pinGuardCurrentTime - pinGuardLastCheck >= 1000)
    {
      pinGuardLastCheck += 1000;

      // check the pins for timeouts
      PinGuard::getInstance()->checkPins();
    }
  }
}

void periodicChecksAndReport()
{
  currentTime = millis();

  // If needed, check the pins and report the value
  if (currentTime < lastReportPin)
  {

    // If the device timer overruns, reset the idle counter
    lastReportPin = millis();
  }
  else
  {
    if ((currentTime - lastReportPin) > reportingPeriodPins)
    {
      lastReportPin = millis();

      // Send the periodic report for pin values
      pinNr = ParameterList::getInstance()->getValue(PIN_REPORT_1_PIN_NR);
      if (pinNr > 0)
      {
        pinValue = analogRead(pinNr);

        Serial.print(COMM_REPORT_PIN_VALUE);
        Serial.print(" P");
        Serial.print(pinNr);
        Serial.print(" V");
        Serial.print(pinValue);
        Serial.println();
      }

      pinNr = ParameterList::getInstance()->getValue(PIN_REPORT_2_PIN_NR);
      if (pinNr > 0)
      {
        pinValue = analogRead(pinNr);

        Serial.print(COMM_REPORT_PIN_VALUE);
        Serial.print(" P");
        Serial.print(pinNr);
        Serial.print(" V");
        Serial.print(pinValue);
        Serial.println();
      }
    }
  }

  // Do periodic checks and feedback
  if (currentTime < lastAction)
  {

    // If the device timer overruns, reset the idle counter
    lastAction = millis();
  }
  else
  {

    if ((currentTime - lastAction) > reportingPeriod)
    {
      // After an idle time, send the idle message
      //Movement::getInstance()->test2();

      if (CurrentState::getInstance()->isEmergencyStop())
      {
        Serial.print(COMM_REPORT_EMERGENCY_STOP);
        CurrentState::getInstance()->printQAndNewLine();

        if (debugMessages)
        {
          Serial.print(COMM_REPORT_COMMENT);
          Serial.print(" Emergency stop engaged");
          CurrentState::getInstance()->printQAndNewLine();
        }
      }
      else
      {
        Serial.print(COMM_REPORT_CMD_IDLE);
        CurrentState::getInstance()->printQAndNewLine();
      }

      Movement::getInstance()->storePosition();
      CurrentState::getInstance()->printPosition();

      Movement::getInstance()->reportEncoders();
      CurrentState::getInstance()->storeEndStops();
      CurrentState::getInstance()->printEndStops();

      // Movement::getInstance()->test();

      if (debugMessages)
      {
        Serial.print(COMM_REPORT_COMMENT);
        Serial.print(" MEM ");
        Serial.print(freeMemory());
        CurrentState::getInstance()->printQAndNewLine();

        Serial.print(COMM_REPORT_COMMENT);
        Serial.print(" IND DUR ");
        Serial.print(interruptDuration);
        Serial.print(" MAX ");
        Serial.print(interruptDurationMax);
        CurrentState::getInstance()->printQAndNewLine();

        //Movement::getInstance()->test();
      }

      if (ParameterList::getInstance()->getValue(PARAM_CONFIG_OK) != 1)
      {
        Serial.print(COMM_REPORT_NO_CONFIG);
        CurrentState::getInstance()->printQAndNewLine();
      }

      cycleCounter++;
      lastAction = millis();
    }
  }
}

void checkSerialInputs()
{
  if (Serial.available())
  {
    // Save current time stamp for timeout actions
    lastAction = millis();

    // Get the input and start processing on receiving 'new line'
    incomingChar = Serial.read();

    // Filter out emergency stop.
    if (!(incomingChar == 69 || incomingChar == 101))
    {
      incomingCommandArray[incomingCommandPointer] = incomingChar;
      incomingCommandPointer++;
    }
    else
    {
      CurrentState::getInstance()->setEmergencyStop();
    }

    // If the string is getting to long, cap it off with a new line and let it process anyway
    if (incomingCommandPointer >= INCOMING_CMD_BUF_SIZE - 1)
    {
      incomingChar = '\n';
      incomingCommandArray[incomingCommandPointer] = incomingChar;
      incomingCommandPointer++;
    }

    if (incomingChar == '\n' || incomingCommandPointer >= INCOMING_CMD_BUF_SIZE)
    {

      //char commandChar[incomingCommandPointer + 1];
      for (int i = 0; i < incomingCommandPointer - 1; i++)
      {
          commandChar[i] = incomingCommandArray[i];
      }
      commandChar[incomingCommandPointer-1] = '\0';

      if (incomingCommandPointer > 1)
      {

        // Report back the received command
        Serial.print(COMM_REPORT_CMD_ECHO);
        Serial.print(" ");
        Serial.print("*");
        Serial.print(commandChar);
        Serial.print("*");
        Serial.print("\r\n");

        // Create a command and let it execute
        Command *command = new Command(commandChar);

        // Log the values if needed for debugging
        if (LOGGING || debugMessages)
        {
          command->print();
        }

        gCodeProcessor->execute(command);

        if (CurrentState::getInstance()->getLastError() == ERR_MOVEMENT_ABORT)
        {
          CurrentState::getInstance()->setLastError(ERR_NO_ERROR);
          CurrentState::getInstance()->resetMovementAbort();
        }

        free(command);

      }

      incomingCommandPointer = 0;
    }
  }
}

void checkEmergencyStop()
{
  // In case of emergency stop, disable movement and
  // shut down the pins used
  if (previousEmergencyStop == false && CurrentState::getInstance()->isEmergencyStop())
  {
    Movement::getInstance()->disableMotorsEmergency();
    PinControl::getInstance()->resetPinsUsed();
    ServoControl::getInstance()->detachServos();
    if (debugMessages)
    {
      Serial.print(COMM_REPORT_COMMENT);
      Serial.print(" Going to safe state");
      CurrentState::getInstance()->printQAndNewLine();
    }
  }
  previousEmergencyStop = CurrentState::getInstance()->isEmergencyStop();
}

void checkParamsChanged()
{
  // Check if parameters are changed, and if so load the new settings
  if (lastParamChangeNr != ParameterList::getInstance()->paramChangeNumber())
  {
    lastParamChangeNr = ParameterList::getInstance()->paramChangeNumber();

    if (debugMessages)
    {
      Serial.print(COMM_REPORT_COMMENT);
      Serial.print(" loading parameters");
      CurrentState::getInstance()->printQAndNewLine();
    }


    #if defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_EXP_V22) || defined(FARMDUINO_EXP_V22) || defined(FARMDUINO_V30) || defined(FARMDUINO_V32)
      stopInterrupt();
      if (lastTmcParamChangeNr != ParameterList::getInstance()->tmcParamChangeNumber())
      {
        lastTmcParamChangeNr = ParameterList::getInstance()->tmcParamChangeNumber();
        Movement::getInstance()->loadSettingsTMC2130();
      }
      restartInterrupt();
    #endif

    Movement::getInstance()->loadSettings();
    PinGuard::getInstance()->loadConfig();
  }
}

void checkEncoders()
{

  #if defined(FARMDUINO_V14) || defined(FARMDUINO_V30) || defined(FARMDUINO_V32)
    // Check encoders out of interrupt for farmduino 1.4
    Movement::getInstance()->checkEncoders();
  #endif

}

// Set pins input output
#ifdef RAMPS_V14
void setPinInputOutput()
{

  // pin input/output settings
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);
  pinMode(E_STEP_PIN, OUTPUT);
  pinMode(E_DIR_PIN, OUTPUT);
  pinMode(E_ENABLE_PIN, OUTPUT);
  pinMode(X_MIN_PIN, INPUT_PULLUP);
  pinMode(X_MAX_PIN, INPUT_PULLUP);

  pinMode(X_ENCDR_A, INPUT_PULLUP);
  pinMode(X_ENCDR_B, INPUT_PULLUP);
  pinMode(X_ENCDR_A_Q, INPUT_PULLUP);
  pinMode(X_ENCDR_B_Q, INPUT_PULLUP);

  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);
  pinMode(Y_MIN_PIN, INPUT_PULLUP);
  pinMode(Y_MAX_PIN, INPUT_PULLUP);

  pinMode(Y_ENCDR_A, INPUT_PULLUP);
  pinMode(Y_ENCDR_B, INPUT_PULLUP);
  pinMode(Y_ENCDR_A_Q, INPUT_PULLUP);
  pinMode(Y_ENCDR_B_Q, INPUT_PULLUP);

  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_ENABLE_PIN, OUTPUT);
  pinMode(Z_MIN_PIN, INPUT_PULLUP);
  pinMode(Z_MAX_PIN, INPUT_PULLUP);

  pinMode(Z_ENCDR_A, INPUT_PULLUP);
  pinMode(Z_ENCDR_B, INPUT_PULLUP);
  pinMode(Z_ENCDR_A_Q, INPUT_PULLUP);
  pinMode(Z_ENCDR_B_Q, INPUT_PULLUP);

  pinMode(HEATER_0_PIN, OUTPUT);
  pinMode(HEATER_1_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(HEATER_0_PIN, LOW);
  digitalWrite(HEATER_1_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  pinMode(UTM_C, INPUT_PULLUP);
  pinMode(UTM_D, INPUT_PULLUP);
  pinMode(UTM_E, INPUT_PULLUP);
  pinMode(UTM_F, INPUT_PULLUP);
  pinMode(UTM_G, INPUT_PULLUP);
  pinMode(UTM_H, INPUT_PULLUP);
  pinMode(UTM_I, INPUT_PULLUP);
  pinMode(UTM_J, INPUT_PULLUP);
  pinMode(UTM_K, INPUT_PULLUP);
  pinMode(UTM_L, INPUT_PULLUP);

  // Aux 1 pins to safer state
  pinMode(AUX1_00, INPUT_PULLUP);
  pinMode(AUX1_01, INPUT_PULLUP);
  pinMode(AUX1_57, INPUT_PULLUP);
  pinMode(AUX1_58, INPUT_PULLUP);

  // Aux 3 pins to safer state
  pinMode(AUX3_49, INPUT_PULLUP);
  pinMode(AUX3_50, INPUT_PULLUP);
  pinMode(AUX3_51, INPUT_PULLUP);

  // Aux 4 pins to safer state
  pinMode(AUX4_43, INPUT_PULLUP);
  pinMode(AUX4_45, INPUT_PULLUP);
  pinMode(AUX4_47, INPUT_PULLUP);
  pinMode(AUX4_32, INPUT_PULLUP);

  pinMode(SERVO_0_PIN, OUTPUT);
  pinMode(SERVO_1_PIN, OUTPUT);
  pinMode(SERVO_2_PIN, OUTPUT);
  pinMode(SERVO_3_PIN, OUTPUT);

  digitalWrite(SERVO_0_PIN, LOW);
  digitalWrite(SERVO_1_PIN, LOW);
  digitalWrite(SERVO_2_PIN, LOW);
  digitalWrite(SERVO_3_PIN, LOW);

  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Set input/output");
  Serial.print(CRLF);
}
#endif

#if defined(FARMDUINO_V10) || defined(FARMDUINO_V14)
void setPinInputOutput()
{

  // pin input/output settings
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);

  pinMode(E_STEP_PIN, OUTPUT);
  pinMode(E_DIR_PIN, OUTPUT);
  pinMode(E_ENABLE_PIN, OUTPUT);
  pinMode(X_MIN_PIN, INPUT_PULLUP);
  pinMode(X_MAX_PIN, INPUT_PULLUP);

  pinMode(X_ENCDR_A, INPUT_PULLUP);
  pinMode(X_ENCDR_B, INPUT_PULLUP);
  //pinMode(X_ENCDR_A_Q, INPUT_PULLUP);
  //pinMode(X_ENCDR_B_Q, INPUT_PULLUP);

  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);
  pinMode(Y_MIN_PIN, INPUT_PULLUP);
  pinMode(Y_MAX_PIN, INPUT_PULLUP);

  pinMode(Y_ENCDR_A, INPUT_PULLUP);
  pinMode(Y_ENCDR_B, INPUT_PULLUP);
  //pinMode(Y_ENCDR_A_Q, INPUT_PULLUP);
  //pinMode(Y_ENCDR_B_Q, INPUT_PULLUP);

  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_ENABLE_PIN, OUTPUT);
  pinMode(Z_MIN_PIN, INPUT_PULLUP);
  pinMode(Z_MAX_PIN, INPUT_PULLUP);

  pinMode(Z_ENCDR_A, INPUT_PULLUP);
  pinMode(Z_ENCDR_B, INPUT_PULLUP);
  //pinMode(Z_ENCDR_A_Q, INPUT_PULLUP);
  //pinMode(Z_ENCDR_B_Q, INPUT_PULLUP);

  pinMode(AUX_STEP_PIN, OUTPUT);
  pinMode(AUX_DIR_PIN, OUTPUT);
  pinMode(AUX_ENABLE_PIN, OUTPUT);
  digitalWrite(AUX_ENABLE_PIN, HIGH);

  pinMode(LED_PIN, OUTPUT);
  pinMode(VACUUM_PIN, OUTPUT);
  pinMode(WATER_PIN, OUTPUT);
  pinMode(LIGHTING_PIN, OUTPUT);
  pinMode(PERIPHERAL_4_PIN, OUTPUT);
  pinMode(PERIPHERAL_5_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(VACUUM_PIN, LOW);
  digitalWrite(WATER_PIN, LOW);
  digitalWrite(LIGHTING_PIN, LOW);
  digitalWrite(PERIPHERAL_4_PIN, LOW);
  digitalWrite(PERIPHERAL_5_PIN, LOW);

  pinMode(UTM_C, INPUT_PULLUP);
  pinMode(UTM_D, INPUT_PULLUP);
  if (UTM_E > 0) { pinMode(UTM_E, INPUT_PULLUP); };
  if (UTM_F > 0) { pinMode(UTM_F, INPUT_PULLUP); };
  if (UTM_G > 0) { pinMode(UTM_G, INPUT_PULLUP); };
  if (UTM_H > 0) { pinMode(UTM_H, INPUT_PULLUP); };
  if (UTM_I > 0) { pinMode(UTM_I, INPUT_PULLUP); };
  if (UTM_J > 0) { pinMode(UTM_J, INPUT_PULLUP); };
  if (UTM_K > 0) { pinMode(UTM_K, INPUT_PULLUP); };
  if (UTM_L > 0) { pinMode(UTM_L, INPUT_PULLUP); };

  pinMode(SERVO_0_PIN, OUTPUT);
  pinMode(SERVO_1_PIN, OUTPUT);
  pinMode(SERVO_2_PIN, OUTPUT);
  pinMode(SERVO_3_PIN, OUTPUT);

  digitalWrite(SERVO_0_PIN, LOW);
  digitalWrite(SERVO_1_PIN, LOW);
  digitalWrite(SERVO_2_PIN, LOW);
  digitalWrite(SERVO_3_PIN, LOW);

  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Set input/output");
  Serial.print(CRLF);

#if defined(FARMDUINO_V14)

    reportingPeriod = 500;

    pinMode(READ_ENA_PIN, INPUT_PULLUP);
    pinMode(NSS_PIN, OUTPUT);
    digitalWrite(NSS_PIN, HIGH);

    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    SPI.begin();
#endif
}
#endif

#if defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_EXP_V22) || defined(FARMDUINO_V30) || defined(FARMDUINO_V32)
void setPinInputOutput()
{
  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Set input/output farmbot express");
  Serial.print(CRLF);

  // Motor step, direction and pin is set up using the control chip library
  // This board also does not use encoders

  pinMode(X_ENABLE_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_MIN_PIN, INPUT_PULLUP);
  pinMode(X_MAX_PIN, INPUT_PULLUP);

  digitalWrite(X_ENABLE_PIN, HIGH);
  digitalWrite(X_DIR_PIN, LOW);
  digitalWrite(X_STEP_PIN, LOW);

  pinMode(Y_ENABLE_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_MIN_PIN, INPUT_PULLUP);
  pinMode(Y_MAX_PIN, INPUT_PULLUP);

  digitalWrite(Y_ENABLE_PIN, HIGH);
  digitalWrite(Y_DIR_PIN, LOW);
  digitalWrite(Y_STEP_PIN, LOW);

  pinMode(Z_ENABLE_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_MIN_PIN, INPUT_PULLUP);
  pinMode(Z_MAX_PIN, INPUT_PULLUP);

  digitalWrite(Z_ENABLE_PIN, HIGH);
  digitalWrite(Z_DIR_PIN, LOW);
  digitalWrite(Z_STEP_PIN, LOW);

  pinMode(E_ENABLE_PIN, OUTPUT);
  pinMode(E_DIR_PIN, OUTPUT);
  pinMode(E_STEP_PIN, OUTPUT);

  digitalWrite(E_ENABLE_PIN, HIGH);
  digitalWrite(E_DIR_PIN, LOW);
  digitalWrite(E_STEP_PIN, LOW);

  #if defined(FARMDUINO_V30) || defined(FARMDUINO_V32)
    pinMode(AUX_STEP_PIN, OUTPUT);
    pinMode(AUX_DIR_PIN, OUTPUT);
    pinMode(AUX_ENABLE_PIN, OUTPUT);

  digitalWrite(AUX_STEP_PIN, LOW);
  digitalWrite(AUX_DIR_PIN, LOW);
  digitalWrite(AUX_ENABLE_PIN, LOW);
  #endif

  pinMode(LED_PIN, OUTPUT);
  pinMode(VACUUM_PIN, OUTPUT);
  pinMode(WATER_PIN, OUTPUT);
  pinMode(LIGHTING_PIN, OUTPUT);
  #if defined(FARMDUINO_V30) || defined(FARMDUINO_V32)
    pinMode(PERIPHERAL_4_PIN, OUTPUT);
    pinMode(PERIPHERAL_5_PIN, OUTPUT);
  #endif
  #if defined(FARMDUINO_V32)
    pinMode(ROTARY_TOOL_FORWARD, OUTPUT);
    pinMode(ROTARY_TOOL_REVERSE, OUTPUT);
  #endif

  digitalWrite(LED_PIN, LOW);
  digitalWrite(VACUUM_PIN, LOW);
  digitalWrite(WATER_PIN, LOW);
  digitalWrite(LIGHTING_PIN, LOW);
  #if defined(FARMDUINO_V30) || defined(FARMDUINO_V32)
    digitalWrite(PERIPHERAL_4_PIN, LOW);
    digitalWrite(PERIPHERAL_5_PIN, LOW);
  #endif
  #if defined(FARMDUINO_V32)
    digitalWrite(ROTARY_TOOL_FORWARD, LOW);
    digitalWrite(ROTARY_TOOL_REVERSE, LOW);
  #endif

  pinMode(LIGHTING_CURRENT_PIN, INPUT_PULLUP);
  pinMode(WATER_CURRENT_PIN, INPUT_PULLUP);
  pinMode(VACUUM_CURRENT_PIN, INPUT_PULLUP);
  #if defined(FARMDUINO_V30) || defined(FARMDUINO_V32)
    pinMode(PERIPHERAL_4_CURRENT_PIN, INPUT_PULLUP);
    pinMode(PERIPHERAL_5_CURRENT_PIN, INPUT_PULLUP);
  #endif
  #if defined(FARMDUINO_V32)
    pinMode(ROTARY_TOOL_CURRENT_PIN, INPUT_PULLUP);
  #endif

  if (UTM_C > 0) { pinMode(UTM_C, INPUT_PULLUP); };
  if (UTM_D > 0) { pinMode(UTM_D, INPUT_PULLUP); };
  if (UTM_E > 0) { pinMode(UTM_E, INPUT_PULLUP); };
  if (UTM_F > 0) { pinMode(UTM_F, INPUT_PULLUP); };
  if (UTM_G > 0) { pinMode(UTM_G, INPUT_PULLUP); };
  if (UTM_H > 0) { pinMode(UTM_H, INPUT_PULLUP); };
  if (UTM_I > 0) { pinMode(UTM_I, INPUT_PULLUP); };
  if (UTM_J > 0) { pinMode(UTM_J, INPUT_PULLUP); };
  if (UTM_K > 0) { pinMode(UTM_K, INPUT_PULLUP); };
  if (UTM_L > 0) { pinMode(UTM_L, INPUT_PULLUP); };

  pinMode(SERVO_0_PIN, OUTPUT);
  pinMode(SERVO_1_PIN, OUTPUT);
  pinMode(SERVO_2_PIN, OUTPUT);
  pinMode(SERVO_3_PIN, OUTPUT);

  digitalWrite(SERVO_0_PIN, LOW);
  digitalWrite(SERVO_1_PIN, LOW);
  digitalWrite(SERVO_2_PIN, LOW);
  digitalWrite(SERVO_3_PIN, LOW);

  #if defined(FARMDUINO_V30) || defined(FARMDUINO_V32)

    reportingPeriod = 500;

    pinMode(READ_ENA_PIN, INPUT_PULLUP);
    pinMode(NSS_PIN, OUTPUT);
    digitalWrite(NSS_PIN, HIGH);

    SPI.setBitOrder(MSBFIRST);
    #if defined(FARMDUINO_V32)
      SPI.setDataMode(SPI_MODE3);
    #else
      SPI.setDataMode(SPI_MODE0);
    #endif
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    SPI.begin();

  #endif
}
#endif

// other initialisation functions
void startSerial()
{
  Serial.begin(115200);
  delay(100);
  while (!Serial);

  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Serial connection started");
  Serial.print(CRLF);
}

void startInterrupt()
{
  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Start interrupt");
  Serial.print(CRLF);

  // Start the interrupt used for moving
  // Interrupt management code library written by Paul Stoffregen
  // The default time 100 micro seconds

  Timer1.attachInterrupt(interrupt);
  Timer1.initialize(MOVEMENT_INTERRUPT_SPEED);
  Timer1.start();
}

void stopInterrupt()
{
  Timer1.stop();
}

void restartInterrupt()
{
  Timer1.start();
}

void homeOnBoot()
{
  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Check homing on boot");
  Serial.print(CRLF);

  if
  (
  (ParameterList::getInstance()->getValue(PARAM_CONFIG_OK) == 1) &&
  (ParameterList::getInstance()->getValue(MOVEMENT_HOME_AT_BOOT_Z) == 1)
  )
  {
    Serial.print("R99 HOME Z ON STARTUP\r\n");
    Movement::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, false, false, true);
  }

  if
  (
  (ParameterList::getInstance()->getValue(PARAM_CONFIG_OK) == 1) &&
  (ParameterList::getInstance()->getValue(MOVEMENT_HOME_AT_BOOT_Y) == 1)
  )
  {
    Serial.print("R99 HOME Y ON STARTUP\r\n");
    Movement::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, false, true, false);
  }

  if
  (
  (ParameterList::getInstance()->getValue(PARAM_CONFIG_OK) == 1) &&
  (ParameterList::getInstance()->getValue(MOVEMENT_HOME_AT_BOOT_X) == 1)
  )
  {
    Serial.print("R99 HOME X ON STARTUP\r\n");
    Movement::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, true, false, false);
  }
}

void startMotor()
{
  // Start the motor handling
  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Set motor enables off");
  Serial.print(CRLF);

  digitalWrite(X_ENABLE_PIN, HIGH);
  digitalWrite(E_ENABLE_PIN, HIGH);
  digitalWrite(Y_ENABLE_PIN, HIGH);
  digitalWrite(Z_ENABLE_PIN, HIGH);
}

void loadMovementSetting()
{

  // Load motor settings
  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Load movement settings");
  Serial.print(CRLF);

  Movement::getInstance()->loadSettings();
}

void readParameters()
{
  // Dump all values to the serial interface
  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Read parameters");
  Serial.print(CRLF);

  ParameterList::getInstance()->readAllValues();
}

void startPinGuard()
{
  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Start pin guard");
  Serial.print(CRLF);

  // Get the settings for the pin guard
  PinGuard::getInstance()->loadConfig();

  pinGuardCurrentTime = millis();
  pinGuardLastCheck = millis();
}

void startServo()
{

  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Start servo");
  Serial.print(CRLF);

  ServoControl::getInstance()->attach();
}

#if defined(FARMDUINO_EXP_V20) || defined(FARMDUINO_EXP_V22) || defined(FARMDUINO_V30) || defined(FARMDUINO_V32)

void loadTMC2130drivers()
{
  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Load TMC drivers");
  Serial.print(CRLF);
}

void loadTMC2130parameters()
{
  // Initialize the drivers
  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Load TMC2130 parameters");
  Serial.print(CRLF);

  Movement::getInstance()->loadSettingsTMC2130();
}

void startupTmc()
{

  // Initialize the drivers
  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Init TMC2130 drivers");
  Serial.print(CRLF);

  Movement::getInstance()->initTMC2130();

  //loadTMC2130parameters();

  //TMC2130X.init();
  //TMC2130Y.init();
  //TMC2130Z.init();
  //TMC2130E.init();

  //Movement::getInstance()->initTMC2130();
}
#endif

void initLastAction()
{
  // Initialize the inactivity check
  lastAction = millis();
}

void setupTestForDebug()
{

  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Setup Debug");
  Serial.print(CRLF);

  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);
  digitalWrite(Z_ENABLE_PIN, LOW);
  digitalWrite(E_ENABLE_PIN, LOW);

  //digitalWrite(X_STEP_PIN, false);
  //digitalWrite(Y_STEP_PIN, false);
  //digitalWrite(Z_STEP_PIN, false);
  //digitalWrite(E_STEP_PIN, false);

  //digitalWrite(X_DIR_PIN, false);
  //digitalWrite(Y_DIR_PIN, false);
  //digitalWrite(Z_DIR_PIN, false);
  //digitalWrite(E_DIR_PIN, false);

  //digitalWrite(X_ENABLE_PIN, true);
  //digitalWrite(Y_ENABLE_PIN, true);
  //digitalWrite(Z_ENABLE_PIN, true);
  //digitalWrite(E_ENABLE_PIN, true);

  //loadTMC2130ParametersMotor(&controllerTMC2130_X, 8, 500, 0);  delay(100);
  //loadTMC2130ParametersMotor(&controllerTMC2130_Y, 8, 500, 0);  delay(100);
  //loadTMC2130ParametersMotor(&controllerTMC2130_Z, 8, 500, 0);  delay(100);
  //loadTMC2130ParametersMotor(&controllerTMC2130_E, 8, 500, 0);  delay(100);

/*
  Serial.println("Init");

  loadTMC2130ParametersMotor(&TMC2130X, 8, 200, 0);
  loadTMC2130ParametersMotor(&TMC2130Y, 8, 200, 0);
  loadTMC2130ParametersMotor(&TMC2130Z, 8, 200, 0);
  loadTMC2130ParametersMotor(&TMC2130E, 8, 200, 0);
  */

}


bool left = false;
int missedX = 0;
int missedY = 0;
int missedZ = 0;
int missedE = 0;

void runTestForDebug()
{

  /*
  currentTime = millis();
  if (currentTime < lastAction)
  {

    // If the device timer overruns, reset the idle counter
    lastAction = millis();
  }
  else
  {

    if ((currentTime - lastAction) > reportingPeriod)
    {
      blinkLed();
      //Serial.print(".");
      lastAction = currentTime;

      Serial.print(">");
      Serial.print(" X = ");
      Serial.print(missedX);
      Serial.print(" Y = ");
      Serial.print(missedY);
      Serial.print(" Z = ");
      Serial.print(missedZ);
      Serial.print(" E = ");
      Serial.print(missedE);
      Serial.print(CRLF);

      if (left) {
        left = false;
        //digitalWrite(X_DIR_PIN, LOW);
        //digitalWrite(Y_DIR_PIN, LOW);
        //digitalWrite(Z_DIR_PIN, LOW);
        //digitalWrite(E_DIR_PIN, LOW);
      }
      else {
        left = true;
        //digitalWrite(X_DIR_PIN, HIGH);
        //digitalWrite(Y_DIR_PIN, HIGH);
        //digitalWrite(Z_DIR_PIN, HIGH);
        //digitalWrite(E_DIR_PIN, HIGH);
      }

    }
  }
  */

  /*
  // make a step
  digitalWrite(X_STEP_PIN, HIGH);
  digitalWrite(Y_STEP_PIN, HIGH);
  digitalWrite(Z_STEP_PIN, HIGH);
  digitalWrite(E_STEP_PIN, HIGH);
  delayMicroseconds(100);

  digitalWrite(X_STEP_PIN, LOW);
  digitalWrite(Y_STEP_PIN, LOW);
  digitalWrite(Z_STEP_PIN, LOW);
  digitalWrite(E_STEP_PIN, LOW);
  delayMicroseconds(100);

  bool stallGuard = false;
  bool standStill = false;
  uint8_t status = 0;
  */

  Movement::getInstance()->test();

/*
  TMC2130X.read_STAT();
  TMC2130Y.read_STAT();
  TMC2130Z.read_STAT();
  TMC2130E.read_STAT();

  status = TMC2130X.getStatus();
  stallGuard = status & FB_TMC_SPISTATUS_STALLGUARD_MASK ? true : false;
  standStill = status & FB_TMC_SPISTATUS_STANDSTILL_MASK ? true : false;
  if (stallGuard || standStill) { missedX++;}

  status = TMC2130Y.getStatus();
  stallGuard = status & FB_TMC_SPISTATUS_STALLGUARD_MASK ? true : false;
  standStill = status & FB_TMC_SPISTATUS_STANDSTILL_MASK ? true : false;
  if (stallGuard || standStill) { missedY++; }

  status = TMC2130Z.getStatus();
  stallGuard = status & FB_TMC_SPISTATUS_STALLGUARD_MASK ? true : false;
  standStill = status & FB_TMC_SPISTATUS_STANDSTILL_MASK ? true : false;
  if (stallGuard || standStill) { missedZ++; }

  status = TMC2130E.getStatus();
  stallGuard = status & FB_TMC_SPISTATUS_STALLGUARD_MASK ? true : false;
  standStill = status & FB_TMC_SPISTATUS_STANDSTILL_MASK ? true : false;
  if (stallGuard || standStill) { missedE++; }
*/

  //if (TMC2130X.isStandstill() || TMC2130X.isStallguard()) {missedX++;}
  //if (TMC2130Y.isStandstill() || TMC2130Y.isStallguard()) {missedY++;}
  //if (TMC2130Z.isStandstill() || TMC2130Z.isStallguard()) {missedZ++;}
  //if (TMC2130E.isStandstill() || TMC2130E.isStallguard()) {missedE++;}

  //Movement::getInstance()->test();
  //delayMicroseconds(100);

  //if (debugInterrupt)
  //{
  //  Movement::getInstance()->handleMovementInterrupt();
  //}
}
