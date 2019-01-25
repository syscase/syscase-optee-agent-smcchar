MODULE_NAME = smcchar
SRC += vendor/memmem.c
SRC += $(SYSCASE)/vendor/jsmn/jsmn.c
SRC += $(SYSCASE)/src/core/afl_call.c
SRC += $(SYSCASE)/src/core/test_run.c
SRC += $(SYSCASE)/src/core/buffer.c
SRC += $(SYSCASE)/src/core/parse_state.c
SRC += $(SYSCASE)/src/optee/smc/parse_handler.c
SRC += $(SYSCASE)/src/linux/smc/common.c
SRC += $(SYSCASE)/src/core/test_case.c
SRC += $(SYSCASE)/src/core/argument/number.c
SRC += $(SYSCASE)/src/core/argument/alloc.c
SRC += $(SYSCASE)/src/core/argument/buffer.c
SRC += $(SYSCASE)/src/core/argument/length.c
SRC += $(SYSCASE)/src/core/argument/vector_32.c
SRC += $(SYSCASE)/src/core/argument/vector_64.c
SRC += $(SYSCASE)/src/core/argument/reference.c
SRC += $(SYSCASE)/src/optee/smc/argument/msg.c
SRC += $(SYSCASE)/src/core/utils.c
SRC += smcchar_main.c

KDIR ?= /lib/modules/$(shell uname -r)/build/

$(MODULE_NAME)-objs = $(SRC:.c=.o)
obj-m += $(MODULE_NAME).o
# Compile with `-D SYSCASE_DUMMY` to disable AFL / SMC calls
EXTRA_CFLAGS=-I$(PWD)/include -I$(PWD)/$(SYSCASE)/vendor/jsmn -I$(PWD)/$(SYSCASE)/include/core -I$(PWD)/$(SYSCASE)/include/linux -I$(PWD)/$(SYSCASE)/include/optee -std=gnu99 -D SYSCASE_SMC=y
ccflags-y += $(EXTRA_CFLAGS)

all:
	make -C $(KDIR) M=$(PWD) modules
	$(CC) -I include smcchar_test.c smc_call.c -o smcchar_test
clean:
	make -C $(KDIR) M=$(PWD) clean
	rm smcchar_test
