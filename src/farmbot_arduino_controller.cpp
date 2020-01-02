// Do not remove the include below
#include "farmbot_arduino_controller.h"

#if !defined(FARMDUINO_EXP_V20)
#include "TimerOne.h"
#endif

bool stepperInit = false;
bool stepperFlip = false;

static char commandEndChar = 0x0A;
char commandChar[INCOMING_CMD_BUF_SIZE + 1];
//String commandString = "";
char incomingChar = 0;
char incomingCommandArray[INCOMING_CMD_BUF_SIZE];
int incomingCommandPointer = 0;

static GCodeProcessor *gCodeProcessor = new GCodeProcessor();

int reportingPeriod = 5000;
unsigned long lastAction;
unsigned long currentTime;
unsigned long cycleCounter = 0;
bool previousEmergencyStop = false;

unsigned long pinGuardLastCheck;
unsigned long pinGuardCurrentTime;

int lastParamChangeNr = 0;

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

#if !defined(FARMDUINO_EXP_V20)
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
#endif

/**/ // unsigned long intrCounter = 0;

#if defined(FARMDUINO_EXP_V20)
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

  // Do periodic checks and feedback
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
      // After an idle time, send the idle message

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

    /*
    #if defined(FARMDUINO_EXP_V20)
    loadTMC2130paraeters();
    #endif
    */

    Movement::getInstance()->loadSettings();
    PinGuard::getInstance()->loadConfig();
  }
}

void checkEncoders()
{
  
  #if defined(FARMDUINO_V14)
    // Check encoders out of interrupt for farmduino 1.4
    Movement::getInstance()->checkEncoders();
  #endif
  
}

void runTestForDebug()
{

  //Serial.print("* ");
  //Serial.print(intrCounter / 1000);
  //Serial.print("\r\n");
  //delay(500);

  //Serial.print("z");
  //blinkLed();
  //delay(500);

  //Serial.print(millis());
  //Serial.print("X");
  //Serial.print("\r\n");
  //delay(1000);

  //blinkLed();

  //Movement::getInstance()->test();
  //delayMicroseconds(100);

  //digitalWrite(LED_PIN, true);
  //delay(250);
  //digitalWrite(LED_PIN, false);
  //delay(250);

  //if (debugInterrupt)
  //{
  //  Movement::getInstance()->handleMovementInterrupt();
  //}
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

#if defined(FARMDUINO_EXP_V20)
void setPinInputOutput()
{

  // Motor step, direction and pin is set up using the control chip library
  // This board also does not use encoders

  pinMode(X_ENABLE_PIN, OUTPUT);
  pinMode(X_MIN_PIN, INPUT_PULLUP);
  pinMode(X_MAX_PIN, INPUT_PULLUP);

  pinMode(E_ENABLE_PIN, OUTPUT);

  pinMode(Y_ENABLE_PIN, OUTPUT);
  pinMode(Y_MIN_PIN, INPUT_PULLUP);
  pinMode(Y_MAX_PIN, INPUT_PULLUP);

  pinMode(Z_ENABLE_PIN, OUTPUT);
  pinMode(Z_MIN_PIN, INPUT_PULLUP);
  pinMode(Z_MAX_PIN, INPUT_PULLUP);

  pinMode(AUX_STEP_PIN, OUTPUT);
  pinMode(AUX_DIR_PIN, OUTPUT);
  pinMode(AUX_ENABLE_PIN, OUTPUT);

  pinMode(LED_PIN, OUTPUT);
  pinMode(VACUUM_PIN, OUTPUT);
  pinMode(WATER_PIN, OUTPUT);
  pinMode(LIGHTING_PIN, OUTPUT);
  pinMode(PERIPHERAL_4_PIN, OUTPUT);
  pinMode(PERIPHERAL_5_PIN, OUTPUT);

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

  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Set input/output");
  Serial.print(CRLF);
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

  #if !defined(FARMDUINO_EXP_V20)
    Timer1.attachInterrupt(interrupt);
    Timer1.initialize(MOVEMENT_INTERRUPT_SPEED);
    Timer1.start();
  #endif

  #if defined(FARMDUINO_EXP_V20)
    Serial.println("set timer");
        TIMSK2 = (TIMSK2 & B11111110) | 0x01; // Enable timer overflow
        TCCR2B = (TCCR2B & B11111000) | 0x01; // Set divider to 1
        OCR2A = 4; // Set overflow to 4 for total of 64 µs
  #endif
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

#if defined(FARMDUINO_EXP_V20)

/**/
//Trinamic_TMC2130 controllerTMC2130_X(X_CHIP_SELECT);
//Trinamic_TMC2130 controllerTMC2130_Y(Y_CHIP_SELECT);
//Trinamic_TMC2130 controllerTMC2130_Z(Z_CHIP_SELECT);
//Trinamic_TMC2130 controllerTMC2130_E(E_CHIP_SELECT);

void loadTMC2130drivers()
{
  Serial.print(COMM_REPORT_COMMENT);
  Serial.print(SPACE);
  Serial.print("Load TMC drivers");
  Serial.print(CRLF);

  /**/
  //TMC2130X.init();
  //TMC2130Y.init();
  //TMC2130Z.init();
  //TMC2130E.init();

  Trinamic_TMC2130 controllerTMC2130_X = Trinamic_TMC2130(X_CHIP_SELECT);
  Trinamic_TMC2130 controllerTMC2130_Y = Trinamic_TMC2130(Y_CHIP_SELECT);
  Trinamic_TMC2130 controllerTMC2130_Z = Trinamic_TMC2130(Z_CHIP_SELECT);
  Trinamic_TMC2130 controllerTMC2130_E = Trinamic_TMC2130(E_CHIP_SELECT);

  TMC2130X = &controllerTMC2130_X;
  TMC2130Y = &controllerTMC2130_Y;
  TMC2130Z = &controllerTMC2130_Z;
  TMC2130E = &controllerTMC2130_E;
}

void loadTMC2130parameters()
{
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
}
#endif

void initLastAction()
{
  // Initialize the inactivity check
  lastAction = millis();
}