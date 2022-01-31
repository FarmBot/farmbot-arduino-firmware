#ifndef FARMBOT_BOARD_ID

  // Farmbot Genesis 1.2 (using RAMPS board)
  #define RAMPS_V14

  // Farmbot Genesis 1.3
  //#define FARMDUINO_V10

  // Farmbot Genesis 1.4
  //#define FARMDUINO_V14

  // Farmbot Genesis 1.5
  //#define FARMDUINO_V30

  // Farmbot Genesis 1.6
  //#define FARMDUINO_V32

  // Farmbot Express 1.0
  //#define FARMDUINO_EXP_V20

  // Farmbot Express 1.1
  //#define FARMDUINO_EXP_V22

#else

  #undef RAMPS_V14
  #undef FARMDUINO_V10
  #undef FARMDUINO_V14
  #undef FARMDUINO_V30
  #undef FARMDUINO_V32
  #undef FARMDUINO_EXP_V20
  #undef FARMDUINO_EXP_V22

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
  #elif FARMBOT_BOARD_ID == 5
    #define FARMDUINO_V32
  #elif FARMBOT_BOARD_ID == 6
    #define FARMDUINO_EXP_V22
  #endif

#endif
