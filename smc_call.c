#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "syscase/smcchar/api.h"
#include "syscase/smcchar/smc_call.h"

sc_u_long smc_call(char *input, sc_u_long input_size, int flags) {
  int ret, fd;
  sc_u_long result;
  
  fd = open("/dev/smcchar", O_RDWR);
  if (fd < 0){
    perror("Failed to open /dev/smcchar");
    return 1;
  }
  
  printf("Set flags: %d\n", flags);
  ret = pwrite(fd, &flags, SMC_FLAGS_SIZE, SMC_SET_FLAGS);
  if (ret < 0){
    perror("Failed to set flags!");
    return 1;
  }

  printf("Set call with size: %lu\n", input_size);
  ret = pwrite(fd, input, input_size, SMC_CALL);
  if (ret < 0){
    perror("Failed to set call input!");
    return 1;
  }

  printf("Reading result\n");
  ret = pread(fd, &result, SMC_RESULT_SIZE, SMC_RESULT);
  if (ret < 0){
    perror("Failed to read SMC_RESULT.");
    return 1;
  }

  return result;
}
