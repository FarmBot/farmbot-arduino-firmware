
#include "PinGuard.h"

static PinGuard *instance;

PinGuard *PinGuard::getInstance()
{
  if (!instance)
  {
    instance = new PinGuard();
  };
  return instance;
};

PinGuard::PinGuard()
{

  pinGuardPin[0] = PinGuardPin();
  pinGuardPin[1] = PinGuardPin();
  pinGuardPin[2] = PinGuardPin();
  pinGuardPin[3] = PinGuardPin();
  pinGuardPin[4] = PinGuardPin();
  loadConfig();
}

void PinGuard::loadConfig()
{
  pinGuardPin[0].loadPinConfig(PIN_GUARD_1_PIN_NR, PIN_GUARD_1_ACTIVE_STATE, PIN_GUARD_1_TIME_OUT);
  pinGuardPin[1].loadPinConfig(PIN_GUARD_2_PIN_NR, PIN_GUARD_2_ACTIVE_STATE, PIN_GUARD_2_TIME_OUT);
  pinGuardPin[2].loadPinConfig(PIN_GUARD_3_PIN_NR, PIN_GUARD_3_ACTIVE_STATE, PIN_GUARD_3_TIME_OUT);
  pinGuardPin[3].loadPinConfig(PIN_GUARD_4_PIN_NR, PIN_GUARD_4_ACTIVE_STATE, PIN_GUARD_4_TIME_OUT);
  pinGuardPin[4].loadPinConfig(PIN_GUARD_5_PIN_NR, PIN_GUARD_5_ACTIVE_STATE, PIN_GUARD_5_TIME_OUT);
}

void PinGuard::checkPins()
{
  pinGuardPin[0].processTick();
  pinGuardPin[1].processTick();
  pinGuardPin[2].processTick();
  pinGuardPin[3].processTick();
  pinGuardPin[4].processTick();
}
