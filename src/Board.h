#ifndef FARMBOT_BOARD_ID

  // Farmbot using RAMPS board
  //#define RAMPS_V14

  //#define FARMDUINO_V10
  //#define FARMDUINO_V14

  // Farmbot Genesis 1.5
  //#define FARMDUINO_V30

  // Farmbot Express
  #define FARMDUINO_EXP_V20

#else

  #undef RAMPS_V14
  #undef FARMDUINO_V10
  #undef FARMDUINO_V14
  #undef FARMDUINO_V30
  #undef FARMDUINO_EXP_V20

  #if FARMBOT_BOARD_ID == 0
    #define RAMPS_V14
  #elif FARMBOT_BOARD_ID == 1
    #define FARMDUINO_V10
  #elif FARMBOT_BOARD_ID == 2
    #define FARMDUINO_V14
  #elif FARMBOT_BOARD_ID == 3
    #define FARMDUINO_EXP_V20
  #elif FARMBOT_BOARD_ID == 4
    #define FARMDUINO_V30
  #endif

#endif
