#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "syscase/smcchar/smc_call.h"

static void read_file(char *filename, char **input, size_t *input_size)
{
  struct stat info;
  if(stat(filename, &info) != 0) {
    printf("Can not read file (stat): %s\n", filename);
    exit(1);
  }
  *input_size = info.st_size;
  
  *input = malloc(*input_size);
  if(*input == NULL) {
    printf("Can not read file (malloc): %s\n", filename);
    exit(1);
  }

  FILE *fp = fopen(filename, "rb");
  if(fp == NULL) {
    printf("Can not read file (fopen): %s\n", filename);
  }

  sc_size_t blocks_read = fread(*input, info.st_size, 1, fp);
  if (blocks_read != 1) {
    printf("Can not read file (fread): %s\n", filename);
    exit(1);
  }

  fclose(fp);
}

int main(int argc, char *argv[])
{
  char *input;
  size_t input_size;
 
  if(argc != 2) {
    printf("Usage: %s INPUT_FILE\n", argv[0]);
    exit(1);
  }
  
  read_file(argv[1], &input, &input_size);
  smc_call(input, input_size, 0);

  free(input);
  return 0;
}
