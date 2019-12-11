#include "TMC2130Stepper.h"
#include "TMC2130Stepper_MACROS.h"

// GCONF
uint32_t TMC2130Stepper::GCONF() { TMC_READ_REG(GCONF); }
void TMC2130Stepper::GCONF(uint32_t input) {
	GCONF_sr = input;
	TMC_WRITE_REG(GCONF);
}

void TMC2130Stepper::I_scale_analog(bool B)			{ TMC_MOD_REG(GCONF, I_SCALE_ANALOG);		}
void TMC2130Stepper::internal_Rsense(bool B)		{ TMC_MOD_REG(GCONF, INTERNAL_RSENSE);		}
void TMC2130Stepper::en_pwm_mode(bool B)			{ TMC_MOD_REG(GCONF, EN_PWM_MODE);			}
void TMC2130Stepper::enc_commutation(bool B)		{ TMC_MOD_REG(GCONF, ENC_COMMUTATION);		}
void TMC2130Stepper::shaft(bool B) 					{ TMC_MOD_REG(GCONF, SHAFT);				}
void TMC2130Stepper::diag0_error(bool B) 			{ TMC_MOD_REG(GCONF, DIAG0_ERROR);			}
void TMC2130Stepper::diag0_otpw(bool B) 			{ TMC_MOD_REG(GCONF, DIAG0_OTPW);			}
void TMC2130Stepper::diag0_stall(bool B) 			{ TMC_MOD_REG(GCONF, DIAG0_STALL);			}
void TMC2130Stepper::diag1_stall(bool B) 			{ TMC_MOD_REG(GCONF, DIAG1_STALL);			}
void TMC2130Stepper::diag1_index(bool B) 			{ TMC_MOD_REG(GCONF, DIAG1_INDEX);			}
void TMC2130Stepper::diag1_onstate(bool B) 			{ TMC_MOD_REG(GCONF, DIAG1_ONSTATE);		}
void TMC2130Stepper::diag1_steps_skipped(bool B) 	{ TMC_MOD_REG(GCONF, DIAG1_STEPS_SKIPPED);	}
void TMC2130Stepper::diag0_int_pushpull(bool B) 	{ TMC_MOD_REG(GCONF, DIAG0_INT_PUSHPULL);	}
void TMC2130Stepper::diag1_pushpull(bool B) 		{ TMC_MOD_REG(GCONF, DIAG1_PUSHPULL);		}
void TMC2130Stepper::small_hysteresis(bool B) 		{ TMC_MOD_REG(GCONF, SMALL_HYSTERESIS);		}
void TMC2130Stepper::stop_enable(bool B) 			{ TMC_MOD_REG(GCONF, STOP_ENABLE);			}
void TMC2130Stepper::direct_mode(bool B) 			{ TMC_MOD_REG(GCONF, DIRECT_MODE);			}

bool TMC2130Stepper::I_scale_analog()				{ TMC_GET_BYTE(GCONF, I_SCALE_ANALOG);		}
bool TMC2130Stepper::internal_Rsense()				{ TMC_GET_BYTE(GCONF, INTERNAL_RSENSE);		}
bool TMC2130Stepper::en_pwm_mode()					{ TMC_GET_BYTE(GCONF, EN_PWM_MODE);			}
bool TMC2130Stepper::enc_commutation()				{ TMC_GET_BYTE(GCONF, ENC_COMMUTATION);		}
bool TMC2130Stepper::shaft() 						{ TMC_GET_BYTE(GCONF, SHAFT);				}
bool TMC2130Stepper::diag0_error() 					{ TMC_GET_BYTE(GCONF, DIAG0_ERROR);			}
bool TMC2130Stepper::diag0_otpw() 					{ TMC_GET_BYTE(GCONF, DIAG0_OTPW);			}
bool TMC2130Stepper::diag0_stall() 					{ TMC_GET_BYTE(GCONF, DIAG0_STALL);			}
bool TMC2130Stepper::diag1_stall() 					{ TMC_GET_BYTE(GCONF, DIAG1_STALL);			}
bool TMC2130Stepper::diag1_index() 					{ TMC_GET_BYTE(GCONF, DIAG1_INDEX);			}
bool TMC2130Stepper::diag1_onstate() 				{ TMC_GET_BYTE(GCONF, DIAG1_ONSTATE);		}
bool TMC2130Stepper::diag1_steps_skipped() 			{ TMC_GET_BYTE(GCONF, DIAG1_STEPS_SKIPPED);	}
bool TMC2130Stepper::diag0_int_pushpull() 			{ TMC_GET_BYTE(GCONF, DIAG0_INT_PUSHPULL);	}
bool TMC2130Stepper::diag1_pushpull() 				{ TMC_GET_BYTE(GCONF, DIAG1_PUSHPULL);		}
bool TMC2130Stepper::small_hysteresis() 			{ TMC_GET_BYTE(GCONF, SMALL_HYSTERESIS);	}
bool TMC2130Stepper::stop_enable() 					{ TMC_GET_BYTE(GCONF, STOP_ENABLE);			}
bool TMC2130Stepper::direct_mode() 					{ TMC_GET_BYTE(GCONF, DIRECT_MODE);			}

/*
bit 18 not implemented:
test_mode 0: 
Normal operation 1:
Enable analog test output on pin DCO. IHOLD[1..0] selects the function of DCO: 
0…2: T120, DAC, VDDH Attention:
Not for user, set to 0 for normal operation! 
*/
