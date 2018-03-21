#ifndef SYSCASE_SMCCHAR_SMC_CALL_H
#define SYSCASE_SMCCHAR_SMC_CALL_H

#include "syscase/types.h"

sc_u_long smc_call(char *input, sc_u_long input_size, int trace);

#endif /*SYSCASE_SMCCHAR_SMC_CALL_H*/

