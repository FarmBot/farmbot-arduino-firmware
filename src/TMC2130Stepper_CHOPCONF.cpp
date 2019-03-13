#include "TMC2130Stepper.h"
#include "TMC2130Stepper_MACROS.h"

// CHOPCONF
uint32_t TMC2130Stepper::CHOPCONF() { TMC_READ_REG(CHOPCONF); }
void TMC2130Stepper::CHOPCONF(uint32_t input) {
	CHOPCONF_sr = input;
	TMC_WRITE_REG(CHOPCONF);
}

void TMC2130Stepper::toff(		uint8_t B )	{ TMC_MOD_REG(CHOPCONF, TOFF);		}
void TMC2130Stepper::hstrt(		uint8_t B )	{ TMC_MOD_REG(CHOPCONF, HSTRT);		}
void TMC2130Stepper::hend(		uint8_t B )	{ TMC_MOD_REG(CHOPCONF, HEND);		}
void TMC2130Stepper::fd(		uint8_t B )	{ TMC_MOD_REG(CHOPCONF, FD);		}
void TMC2130Stepper::disfdcc(	bool 	B )	{ TMC_MOD_REG(CHOPCONF, DISFDCC);	}
void TMC2130Stepper::rndtf(		bool 	B )	{ TMC_MOD_REG(CHOPCONF, RNDTF);		}
void TMC2130Stepper::chm(		bool 	B )	{ TMC_MOD_REG(CHOPCONF, CHM);		}
void TMC2130Stepper::tbl(		uint8_t B )	{ TMC_MOD_REG(CHOPCONF, TBL);		}
void TMC2130Stepper::vsense(	bool 	B )	{ TMC_MOD_REG(CHOPCONF, VSENSE);	}
void TMC2130Stepper::vhighfs(	bool 	B )	{ TMC_MOD_REG(CHOPCONF, VHIGHFS);	}
void TMC2130Stepper::vhighchm(	bool 	B )	{ TMC_MOD_REG(CHOPCONF, VHIGHCHM);	}
void TMC2130Stepper::sync(		uint8_t B )	{ TMC_MOD_REG(CHOPCONF, SYNC);		}
void TMC2130Stepper::mres(		uint8_t B )	{ TMC_MOD_REG(CHOPCONF, MRES);		}
void TMC2130Stepper::intpol(	bool 	B )	{ TMC_MOD_REG(CHOPCONF, INTPOL);	}
void TMC2130Stepper::dedge(		bool 	B )	{ TMC_MOD_REG(CHOPCONF, DEDGE);		}
void TMC2130Stepper::diss2g(	bool 	B )	{ TMC_MOD_REG(CHOPCONF, DISS2G);	}

uint8_t TMC2130Stepper::toff()		{ TMC_GET_BYTE(CHOPCONF, TOFF);		}
uint8_t TMC2130Stepper::hstrt()		{ TMC_GET_BYTE(CHOPCONF, HSTRT);	}
uint8_t TMC2130Stepper::hend()		{ TMC_GET_BYTE(CHOPCONF, HEND);		}
uint8_t TMC2130Stepper::fd()		{ TMC_GET_BYTE(CHOPCONF, FD);		}
bool 	TMC2130Stepper::disfdcc()	{ TMC_GET_BYTE(CHOPCONF, DISFDCC);	}
bool 	TMC2130Stepper::rndtf()		{ TMC_GET_BYTE(CHOPCONF, RNDTF);	}
bool 	TMC2130Stepper::chm()		{ TMC_GET_BYTE(CHOPCONF, CHM);		}
uint8_t TMC2130Stepper::tbl()		{ TMC_GET_BYTE(CHOPCONF, TBL);		}
bool 	TMC2130Stepper::vsense()	{ TMC_GET_BIT( CHOPCONF, VSENSE);	}
bool 	TMC2130Stepper::vhighfs()	{ TMC_GET_BYTE(CHOPCONF, VHIGHFS);	}
bool 	TMC2130Stepper::vhighchm()	{ TMC_GET_BYTE(CHOPCONF, VHIGHCHM);	}
uint8_t TMC2130Stepper::sync()		{ TMC_GET_BYTE(CHOPCONF, SYNC);		}
uint8_t TMC2130Stepper::mres()		{ TMC_GET_BYTE(CHOPCONF, MRES);		}
bool 	TMC2130Stepper::intpol()	{ TMC_GET_BYTE(CHOPCONF, INTPOL);	}
bool 	TMC2130Stepper::dedge()		{ TMC_GET_BYTE(CHOPCONF, DEDGE);	}
bool 	TMC2130Stepper::diss2g()	{ TMC_GET_BYTE(CHOPCONF, DISS2G);	}

void TMC2130Stepper::hysteresis_end(int8_t value) { hend(value+3); }
int8_t TMC2130Stepper::hysteresis_end() { return hend()-3; };

void TMC2130Stepper::hysteresis_start(uint8_t value) { hstrt(value-1); }
uint8_t TMC2130Stepper::hysteresis_start() { return hstrt()+1; }

void TMC2130Stepper::blank_time(uint8_t value) {
	switch (value) {
		case 16: tbl(0b00); break;
		case 24: tbl(0b01); break;
		case 36: tbl(0b10); break;
		case 54: tbl(0b11); break;
	}
}

uint8_t TMC2130Stepper::blank_time() {
	switch (tbl()) {
		case 0b00: return 16;
		case 0b01: return 24;
		case 0b10: return 36;
		case 0b11: return 54;
	}
	return 0;
}
