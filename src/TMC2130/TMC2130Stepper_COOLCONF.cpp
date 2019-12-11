#include "TMC2130Stepper.h"
#include "TMC2130Stepper_MACROS.h"

// COOLCONF
uint32_t TMC2130Stepper::COOLCONF() { return COOLCONF_sr; }
void TMC2130Stepper::COOLCONF(uint32_t input) {
	COOLCONF_sr = input;
	TMC_WRITE_REG(COOLCONF);
}

void TMC2130Stepper::semin(		uint8_t B )	{ TMC_MOD_REG(COOLCONF, SEMIN);		}
void TMC2130Stepper::seup(		uint8_t B )	{ TMC_MOD_REG(COOLCONF, SEUP);		}
void TMC2130Stepper::semax(		uint8_t B )	{ TMC_MOD_REG(COOLCONF, SEMAX);		}
void TMC2130Stepper::sedn(		uint8_t B )	{ TMC_MOD_REG(COOLCONF, SEDN);		}
void TMC2130Stepper::seimin(	bool 	B )	{ TMC_MOD_REG(COOLCONF, SEIMIN);	}
void TMC2130Stepper::sgt(		int8_t  B )	{ TMC_MOD_REG(COOLCONF, SGT);		}
void TMC2130Stepper::sfilt(		bool 	B )	{ TMC_MOD_REG(COOLCONF, SFILT);		}

uint8_t TMC2130Stepper::semin()	{ TMC_GET_BYTE(COOLCONF, SEMIN);	}
uint8_t TMC2130Stepper::seup()	{ TMC_GET_BYTE(COOLCONF, SEUP);		}
uint8_t TMC2130Stepper::semax()	{ TMC_GET_BYTE(COOLCONF, SEMAX);	}
uint8_t TMC2130Stepper::sedn()	{ TMC_GET_BYTE(COOLCONF, SEDN);		}
bool TMC2130Stepper::seimin()	{ TMC_GET_BYTE(COOLCONF, SEIMIN);	}
//int8_t TMC2130Stepper::sgt()	{ TMC_GET_BYTE(COOLCONF, SGT);		}
bool TMC2130Stepper::sfilt()	{ TMC_GET_BYTE(COOLCONF, SFILT);	}

int8_t TMC2130Stepper::sgt() {
	// Two's complement in a 7bit value
	int8_t val = (COOLCONF()&SGT_bm) >> SGT_bp;
	return val <= 63 ? val : val - 128;
}
