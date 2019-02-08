// Do not remove the include below
#include "farmbot_arduino_controller.h"
#include "pins.h"
#include "Config.h"
#include "StepperControl.h"
#include "ServoControl.h"
#include "PinGuard.h"
#include "TimerOne.h"
#include "MemoryFree.h"
#include "Debug.h"
#include "CurrentState.h"
#include <SPI.h>

static char commandEndChar = 0x0A;
static GCodeProcessor *gCodeProcessor = new GCodeProcessor();

int reportingPeriod = 5000;
unsigned long lastAction;
unsigned long currentTime;
unsigned long cycleCounter = 0;
bool previousEmergencyStop = false;

unsigned long pinGuardLastCheck;
unsigned long pinGuardCurrentTime;

int lastParamChangeNr = 0;

String commandString = "";
char incomingChar = 0;
char incomingCommandArray[INCOMING_CMD_BUF_SIZE];
int incomingCommandPointer = 0;

// Blink led routine used for testing
bool blink = false;
void blinkLed()
{
  blink = !blink;
  digitalWrite(LED_PIN, blink);
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
void interrupt(void)
{
  if (!debugInterrupt)
  {
    //interruptSecondTimer++;

    if (interruptBusy == false)
    {
      //interruptStartTime = micros();

      interruptBusy = true;
      StepperControl::getInstance()->handleMovementInterrupt();

      // Check the actions triggered once per second
      //if (interruptSecondTimer >= 1000000 / MOVEMENT_INTERRUPT_SPEED)
      //{
      //  interruptSecondTimer = 0;
      //  PinGuard::getInstance()->checkPins();
      //  //blinkLed();
      //}

      //interruptStopTime = micros();

      //if (interruptStopTime > interruptStartTime)
      //{
      //  interruptDuration = interruptStopTime - interruptStartTime;
      //}

      //if (interruptDuration > interruptDurationMax)
      //{
      //  interruptDurationMax = interruptDuration;
      //}

      interruptBusy = false;
    }
  }
}

//The setup function is called once at startup of the sketch
void setup()
{

  #ifdef RAMPS_V14

    // Setup pin input/output settings
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

  #endif

  #if defined(FARMDUINO_V10) || defined(FARMDUINO_V14)

    // Setup pin input/output settings
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

  #endif

  digitalWrite(X_ENABLE_PIN, HIGH);
  digitalWrite(E_ENABLE_PIN, HIGH);
  digitalWrite(Y_ENABLE_PIN, HIGH);
  digitalWrite(Z_ENABLE_PIN, HIGH);
  
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
  
  Serial.begin(115200);

  delay(100);

  // Start the motor handling
  //ServoControl::getInstance()->attach();

  // Load motor settings
  StepperControl::getInstance()->loadSettings();

  // Dump all values to the serial interface
  // ParameterList::getInstance()->readAllValues();

  // Get the settings for the pin guard
  PinGuard::getInstance()->loadConfig();
  pinGuardLastCheck = millis();

  // Start the interrupt used for moving
  // Interrupt management code library written by Paul Stoffregen
  // The default time 100 micro seconds

  Timer1.attachInterrupt(interrupt);
  Timer1.initialize(MOVEMENT_INTERRUPT_SPEED);
  Timer1.start();

  // Initialize the inactivity check
  lastAction = millis();

  pinGuardCurrentTime = millis();
  pinGuardLastCheck = millis();

  if
  (
    (ParameterList::getInstance()->getValue(PARAM_CONFIG_OK) == 1) &&
    (ParameterList::getInstance()->getValue(MOVEMENT_HOME_AT_BOOT_Z) == 1)
  )
  {
    Serial.print("R99 HOME Z ON STARTUP\r\n");
    StepperControl::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, false, false, true);
  }

  if
  (
    (ParameterList::getInstance()->getValue(PARAM_CONFIG_OK) == 1) &&
    (ParameterList::getInstance()->getValue(MOVEMENT_HOME_AT_BOOT_Y) == 1)
  )
  {
    Serial.print("R99 HOME Y ON STARTUP\r\n");
    StepperControl::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, false, true, false);
  }

  if
  (
    (ParameterList::getInstance()->getValue(PARAM_CONFIG_OK) == 1) &&
    (ParameterList::getInstance()->getValue(MOVEMENT_HOME_AT_BOOT_X) == 1)
  )
  {
    Serial.print("R99 HOME X ON STARTUP\r\n");
    StepperControl::getInstance()->moveToCoords(0, 0, 0, 0, 0, 0, true, false, false);
  }

  Serial.print("R99 ARDUINO STARTUP COMPLETE\r\n");
}

//char commandIn[100];
char commandChar[INCOMING_CMD_BUF_SIZE + 1];

// The loop function is called in an endless loop
void loop()
{

  if (debugInterrupt)
  {
    StepperControl::getInstance()->handleMovementInterrupt();
  }

  #if defined(FARMDUINO_V14)
    // Check encoders out of interrupt for farmduino 1.4
    StepperControl::getInstance()->checkEncoders();
  #endif

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
        //Command* command = new Command(commandString);
        Command *command = new Command(commandChar);

        //strcpy()
        //commandEcho

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

  // In case of emergency stop, disable movement and
  // shut down the pins used
  if (previousEmergencyStop == false && CurrentState::getInstance()->isEmergencyStop())
  {
    StepperControl::getInstance()->disableMotorsEmergency();
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

  // Check if parameters are changes, and if so load the new settings
  if (lastParamChangeNr != ParameterList::getInstance()->paramChangeNumber())
  {
    lastParamChangeNr = ParameterList::getInstance()->paramChangeNumber();

    if (debugMessages)
    {
      Serial.print(COMM_REPORT_COMMENT);
      Serial.print(" loading parameters");
      CurrentState::getInstance()->printQAndNewLine();
    }

    StepperControl::getInstance()->loadSettings();
    PinGuard::getInstance()->loadConfig();
  }

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

      StepperControl::getInstance()->storePosition();
      CurrentState::getInstance()->printPosition();

      StepperControl::getInstance()->reportEncoders();

      CurrentState::getInstance()->storeEndStops();
      CurrentState::getInstance()->printEndStops();

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

        //StepperControl::getInstance()->checkEncoders();
        //Serial.print(COMM_REPORT_COMMENT);
        //Serial.print(" Cycle ");
        //Serial.print(cycleCounter);
        //CurrentState::getInstance()->printQAndNewLine();


//        Serial.print(COMM_REPORT_COMMENT);
//        Serial.print(" 16 ");
//        Serial.print(PORTH & 0x02);
//        Serial.print(PH4);
//        Serial.print(" ");
//        Serial.print(PINH);
//        Serial.print(" ");
//        Serial.print(digitalRead(16));
//        Serial.print(" ");
//        Serial.print(" 17 ");
//        Serial.print(PORTH & 0x01);
//        Serial.print(PH5);
//        Serial.print(" ");
//        Serial.print(digitalRead(17));
//        CurrentState::getInstance()->printQAndNewLine();

        StepperControl::getInstance()->test();
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
