#ifndef SYSCASE_SMCCHAR_API_H
#define SYSCASE_SMCCHAR_API_H

#define DEVICE_NAME "smcchar"
#define CLASS_NAME "smc"

#define SMC_CALL_SIZE 4096
#define SMC_FLAGS_SIZE sizeof(int)
#define SMC_RESULT_SIZE sizeof(unsigned long)

// Write options
#define SMC_CALL 0
#define SMC_SET_FLAGS 1

// Read options
#define SMC_RESULT 0

#endif /*SYSCASE_SMCCHAR_API_H*/

