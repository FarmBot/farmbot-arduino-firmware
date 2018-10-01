#include "CANbusFunctions.h"

/*

static StepperControl *instance;

StepperControl *StepperControl::getInstance()
{
  if (!instance)
  {
    instance = new StepperControl();
  };
  return instance;
};

 */

static CANbusFunctions *instance;

CANbusFunctions *CANbusFunctions::getInstance()
{
  if (!instance)
  {
    //instance = new CANmaster();
    instance = new CANbusFunctions;
    //CANbusFunctions CANmaster;  // Preinstantiate
  };
  return instance;
};

CANbusFunctions::CANmaster()
//CANbusFunctions::CANbusFunctions()
{
  // Will this work?
  CANbusUpdateCurrentTime = 0;
  CANbusUpdateLastCheck = 0;
  CANbusUpdateInterval = 400;
  updateStagger = 0;
}

void CANbusFunctions::CANbusInit() {

  //Serial.println("CANbusInit start");
  
  bool CANbusInit = false;
  bool CANbusInitFailed = false;
  int CANbusInitRetryAttempt = 0;
  int CANbusInitRetryAttemptLimit = 10;
  
  #ifdef DEBUG_CANBUS
  Serial.println("CAN: broadcast 'I'");
  #endif
  
  if(ENC_X1_CAN_ID > 0)
  {
	  CAN.beginPacket(ENC_X1_CAN_ID);
	  CAN.write('I');
	  CAN.endPacket();

	  delay(50);

    // Process incoming messages:
    // Psudo CAN interrupt code
    while(CAN.parsePacket())
    {
      CANinterrupt(CAN.available());
    }
  }
  else
  {
	  x1ModuleOK = true;
  }
  if(ENC_X2_CAN_ID > 0)
  {
	  CAN.beginPacket(ENC_X2_CAN_ID);
	  CAN.write('I');
	  CAN.endPacket();

	  delay(50);

    // Process incoming messages:
    // Psudo CAN interrupt code
    while(CAN.parsePacket())
    {
      CANinterrupt(CAN.available());
    }
  }
  else
  {
	  x2ModuleOK = true;
  }
  if(ENC_Y_CAN_ID > 0)
  {
	  CAN.beginPacket(ENC_Y_CAN_ID);
	  CAN.write('I');
	  CAN.endPacket();

	  delay(50);

    // Process incoming messages:
    // Psudo CAN interrupt code
    while(CAN.parsePacket())
    {
      CANinterrupt(CAN.available());
    }
  }
  else
  {
	  yModuleOK = true;
  }
  if(ENC_Z_CAN_ID > 0)
  {
	  CAN.beginPacket(ENC_Z_CAN_ID);
	  CAN.write('I');
	  CAN.endPacket();

	  delay(50);

    // Process incoming messages:
    // Psudo CAN interrupt code
    while(CAN.parsePacket())
    {
      CANinterrupt(CAN.available());
    }
  }
  else
  {
	  zModuleOK = true;
  }

  // decode returning messages from remote CANbus modules
  decodeCAN();

  // Clear message received flag
  CANmessageReceived = false;

  // Check to see if all modules returned the acknowledge message  
  // Keep checking until all successful, or retry attempt limit has been reached
  while(!CANbusInit && !CANbusInitFailed)
  {
    // Check buffer for any response messages
    for(int i=0; i<BUFFER_SIZE; i++)
    {
      switch(CANincomingAddr[i])
      {
        case(ENC_X1_CAN_ID):
          // Process CANbus data
          if(CANinstruction[i] == 'K')
          {
    			  if(!x1ModuleOK)
    			  {
      				x1ModuleOK = true;
              #ifdef DEBUG_CANBUS
      				Serial.println("X1 init OK");
             #endif
    			  }
          }
          break;
        case(ENC_X2_CAN_ID):
          // Process CANbus data
          if(CANinstruction[i] == 'K')
          {
			      if(!x2ModuleOK)
			      {
				      x2ModuleOK = true;
              #ifdef DEBUG_CANBUS
				      Serial.println("X2 init OK");
             #endif
			      }
          }
          break;
        case(ENC_Y_CAN_ID):
          //  Process CANbus data
          if(CANinstruction[i] == 'K')
          {
			      if(!yModuleOK)
			      {
				      yModuleOK = true;
              #ifdef DEBUG_CANBUS
				      Serial.println("Y init OK");
              #endif
			      }
          }
          break;
        case(ENC_Z_CAN_ID):
          // Process CANbus data
          if(CANinstruction[i] == 'K')
          {
			      if(!zModuleOK)
			      {
				      zModuleOK = true;
              #ifdef DEBUG_CANBUS
				      Serial.println("Z init OK");
              #endif
			      }
          }
          break;
        default:
          // Unknown address
          break;
      }
    }
    // After checking through the whole buffer, see if all modules responded
    if(x1ModuleOK && x2ModuleOK && yModuleOK && zModuleOK)
    {
      // Init checking complete
      CANbusInit = true;

      #ifdef DEBUG_CANBUS
      Serial.println("CANbus init successful");
      #endif
    }
    else
    {
      // A device did not respond. Resend init command. Increase delay after each reattempt
	  if(!x1ModuleOK)
      {
        CAN.beginPacket(ENC_X1_CAN_ID);
        CAN.write('I');
        CAN.endPacket();
      
        delay(50 + CANbusInitRetryAttempt*50);
      }
      if(!x2ModuleOK)
      {
        CAN.beginPacket(ENC_X2_CAN_ID);
        CAN.write('I');
        CAN.endPacket();
      
        delay(50 + CANbusInitRetryAttempt*50);
      }
      if(!yModuleOK)
      {
        CAN.beginPacket(ENC_Y_CAN_ID);
        CAN.write('I');
        CAN.endPacket();
      
        delay(50 + CANbusInitRetryAttempt*50);
      }
      if(!zModuleOK)
      {
        CAN.beginPacket(ENC_Z_CAN_ID);
        CAN.write('I');
        CAN.endPacket();
      
        delay(50 + CANbusInitRetryAttempt*50);
      }

      // Increment retry counter
      CANbusInitRetryAttempt++;

      // Hit retry limit?
      if(CANbusInitRetryAttempt >= CANbusInitRetryAttemptLimit)
      {
        // Failed to initialise CANbus modules, remove this encoder from runtime?
        //- Set ENCODER_ENABLED_*i*_DEFAULT to 0 (currently a const, process to modify is unknown and untested)
        CANbusInitFailed = true;

        #ifdef DEBUG_CANBUS
        Serial.print("CANbus modules failed to initialise: ");
		    if(!x1ModuleOK)
        {
          Serial.print(ENC_X1_CAN_ID);
          Serial.print(". ");
        }
        if(!x2ModuleOK)
        {
          Serial.print(ENC_X2_CAN_ID);
          Serial.print(". ");
        }
        if(!yModuleOK)
        {
          Serial.print(ENC_Y_CAN_ID);
          Serial.print(". ");
        }
        if(!zModuleOK)
        {
          Serial.print(ENC_Z_CAN_ID);
          Serial.print(". ");
        }
        Serial.println();
        #endif
      }
      
    }
    
  }

  //Serial.println("CANbusInit end");
  return;
}

void CANbusFunctions::decodeCAN()  {  
  // Work through the message buffer

  while(CANworkingIndex != CANbufferIndex)
  {
    // Clear flag
    //messageReceived = false;

    // Increment working index
    CANworkingIndex++;

    if(CANworkingIndex >= BUFFER_SIZE)
    {
      // Roll over
      CANworkingIndex = 0;
    }

    // Save sender address
    CANincomingAddr[CANworkingIndex] = incomingAddressBuffer[CANworkingIndex];

    // Clear buffer
    incomingAddressBuffer[CANworkingIndex] = 0;

    // Determine what kind of mesage was sent
    if(incomingCANinstruction[CANworkingIndex] != 0)
    {
      // Must have been an instruction
      CANinstruction[CANworkingIndex] = incomingCANinstruction[CANworkingIndex];

      // Clear buffer
      incomingCANinstruction[CANworkingIndex] = 0;
	  
	  // Action based on instruction type
	  switch((char)CANinstruction[CANworkingIndex])
	  {
		case('K'):	// OK
			// 
			break;
		case('E'):	// Error
			// Set error flag
			switch(CANincomingAddr[CANworkingIndex])
			{
				case(ENC_X1_CAN_ID):
					// X1
					CANbusError[0] = true;
					break;
				case(ENC_X2_CAN_ID):
					// X2
					CANbusError[1] = true;
					break;
				case(ENC_Y_CAN_ID):
					// Y
					CANbusError[2] = true;
					break;
				case(ENC_Z_CAN_ID):
					// Z
					CANbusError[3] = true;
					break;
				default:
					break;
			}
			break;
		case('W'):	// Warning
			// Set warning flag
			switch(CANincomingAddr[CANworkingIndex])
			{
				case(ENC_X1_CAN_ID):
					// X1
					CANbusWarning[0] = true;
					break;
				case(ENC_X2_CAN_ID):
					// X2
					CANbusWarning[1] = true;
					break;
				case(ENC_Y_CAN_ID):
					// Y
					CANbusWarning[2] = true;
					break;
				case(ENC_Z_CAN_ID):
					// Z
					CANbusWarning[3] = true;
					break;
				default:
					break;
			}
			break;
    case('U'):  // Uninitialised
      // Send initialisation code
      switch(CANincomingAddr[CANworkingIndex])
      {
        case(ENC_X1_CAN_ID):
          // X1
          CAN.beginPacket(ENC_X1_CAN_ID);
          CAN.write('I');
          CAN.endPacket();
          break;
        case(ENC_X2_CAN_ID):
          // X2
          CAN.beginPacket(ENC_X2_CAN_ID);
          CAN.write('I');
          CAN.endPacket();
          break;
        case(ENC_Y_CAN_ID):
          // Y
          CAN.beginPacket(ENC_Y_CAN_ID);
          CAN.write('I');
          CAN.endPacket();
          break;
        case(ENC_Z_CAN_ID):
          // Z
          CAN.beginPacket(ENC_Z_CAN_ID);
          CAN.write('I');
          CAN.endPacket();
          break;
        default:
          break;
      }
      break;
		default:
			// Unknown instruction
      #ifdef DEBUG_CANBUS
      Serial.print("Unknown instruction: ");
      Serial.println((char)CANinstruction[CANworkingIndex]);
      #endif
			break;
	  }
    }
    else
    {
      // Must have sent a 32bit number
      // Read in the 4 byte of data.
      CANencoderVal[CANworkingIndex] = incomingValue[CANworkingIndex][0];
      CANencoderVal[CANworkingIndex] = (CANencoderVal[CANworkingIndex] << 8) + incomingValue[CANworkingIndex][1];
      CANencoderVal[CANworkingIndex] = (CANencoderVal[CANworkingIndex] << 8) + incomingValue[CANworkingIndex][2];
      CANencoderVal[CANworkingIndex] = (CANencoderVal[CANworkingIndex] << 8) + incomingValue[CANworkingIndex][3];

      // Clear buffer
      incomingValue[CANworkingIndex][0] = 0;
      incomingValue[CANworkingIndex][1] = 0;
      incomingValue[CANworkingIndex][2] = 0;
      incomingValue[CANworkingIndex][3] = 0;

      //Serial.print(". CANvalue: ");
      //Serial.println(CANencoderVal[CANworkingIndex]);
	  
  	  // Where does this data go?
  	  if(CANincomingAddr[CANworkingIndex] == ENC_X1_CAN_ID)
  	  {
  		  // Set X1 position
  		  // ----
  	  }
  	  if(CANincomingAddr[CANworkingIndex] == ENC_X2_CAN_ID)
  	  {
  		  // Set X2 position
  		  StepperControl::getInstance()->encoderX.setPositionByCAN(CANencoderVal[CANworkingIndex]);
  	  }
  	  if(CANincomingAddr[CANworkingIndex] == ENC_Y_CAN_ID)
  	  {
  		  // Set Y position
        StepperControl::getInstance()->encoderY.setPositionByCAN(CANencoderVal[CANworkingIndex]);
  	  }
  	  if(CANincomingAddr[CANworkingIndex] == ENC_Z_CAN_ID)
  	  {
  		  // Set Z position
        StepperControl::getInstance()->encoderZ.setPositionByCAN(CANencoderVal[CANworkingIndex]);
  	  }
    }
  }

  return;
}

void CANbusFunctions::encodeCAN(long CANvalue)  {
	
	// Is current position positive, negative or zero
  if(CANvalue > 0)
  {
    // Positive
    CANpacket[0] = (byte)(CANvalue >> 24);
    CANpacket[1] = (byte)(CANvalue >> 16);
    CANpacket[2] = (byte)(CANvalue >> 8);
    CANpacket[3] = (byte)(CANvalue);
  }
  else if(CANvalue < 0)
  {
    // Negative
    CANpacket[0] = ~(byte)(CANvalue >> 24);
    CANpacket[1] = ~(byte)(CANvalue >> 16);
    CANpacket[2] = ~(byte)(CANvalue >> 8);
    CANpacket[3] = ~(byte)(CANvalue - 1);
  }
  else
  {
    // Zero
    CANpacket[0] = 0;
    CANpacket[1] = 0;
    CANpacket[2] = 0;
    CANpacket[3] = 0;
  }

  return;
}

void CANbusFunctions::errorChecking()	{
	// Interpret and process incoming errors or warnings
	if(CANbusWarning[0] || CANbusError[0])
    {
      #ifdef DEBUG_CANBUS
      Serial.println("Warning / Error: X1");
      #endif
      // Request an encoder update
      requestEncoderUpdate(ENC_X1_CAN_ID);

      // Clear flags
      CANbusWarning[0] = false;
      CANbusError[0] = false;
    }
    if(CANbusWarning[1] || CANbusError[1])
    {
      #ifdef DEBUG_CANBUS
      Serial.println("Warning / Error: X2");
      #endif
      // Request an encoder update
      requestEncoderUpdate(ENC_X2_CAN_ID);

      // Clear flags
      CANbusWarning[1] = false;
      CANbusError[1] = false;
    }
    if(CANbusWarning[2] || CANbusError[2])
    {
      #ifdef DEBUG_CANBUS
      Serial.println("Warning / Error: Y");
      #endif
      // Request an encoder update
      requestEncoderUpdate(ENC_Y_CAN_ID);

      // Clear flags
      CANbusWarning[2] = false;
      CANbusError[2] = false;
    }
    if(CANbusWarning[3] || CANbusError[3])
    {
      #ifdef DEBUG_CANBUS
      Serial.println("Warning / Error: Z");
      #endif
      // Request an encoder update
      requestEncoderUpdate(ENC_Z_CAN_ID);

      // Clear flags
      CANbusWarning[3] = false;
      CANbusError[3] = false;
    }
	
}

void CANbusFunctions::requestEncoderUpdate(uint8_t addr) {
  
  // Send RTR (remote transmit request) of 4 bytes (32bit number) - CAN.beginPacket((byte)address, (byte)requested data length, (bool)RTR = true)
  CAN.beginPacket(addr,4,true);
  CAN.endPacket();

  return;
}

void CANbusFunctions::CANinterrupt(int packetSize) {
  // Upon interrupt, move data into software buffer for processing

  // Increment buffer index
  CANbufferIndex++;

  if(CANbufferIndex >= BUFFER_SIZE)
  {
    // Roll over
    CANbufferIndex = 0;
  }

  incomingAddressBuffer[CANbufferIndex] = CAN.packetId();

  if(packetSize == 4)
  {
    // Must be 32bit integer
    incomingValue[CANbufferIndex][0] = CAN.read();
    incomingValue[CANbufferIndex][1] = CAN.read();
    incomingValue[CANbufferIndex][2] = CAN.read();
    incomingValue[CANbufferIndex][3] = CAN.read();
  }
  else if(packetSize == 1)
  {
    // Must be instruction character
    incomingCANinstruction[CANbufferIndex] = CAN.read();
  }
  else
  {
    // Packet size outside normal operation
  }
  
  // Declare message recieved
  CANmessageReceived = true;
}

bool CANbusFunctions::timeToCheckEncoders() {
  // Time to check encoders again?
  CANbusUpdateCurrentTime = millis();
  
  if(CANbusUpdateCurrentTime - CANbusUpdateLastCheck >= CANbusUpdateInterval)
  {
    CANbusUpdateLastCheck = CANbusUpdateCurrentTime;

    // Request position update from CANbus encoder modules
    //StepperControl::getInstance()->checkEncoders();
    // Stagger the requests
    switch(updateStagger)
    {
      case 0:
        StepperControl::getInstance()->encoderX.processEncoder();
        updateStagger++;
        break;
      case 1:
        StepperControl::getInstance()->encoderY.processEncoder();
        updateStagger++;
        break;
      case 2:
        StepperControl::getInstance()->encoderZ.processEncoder();
        updateStagger = 0;
        break;
      default:
        updateStagger = 0;
        break;
    }
    
    return true;    
  }
  else
  {
    return false;
  }
}

