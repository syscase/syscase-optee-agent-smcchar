MODULE_NAME = smcchar
SRC += vendor/memmem.c
SRC += $(SYSCASE)/afl_call.c
SRC += $(SYSCASE)/test_run.c
SRC += $(SYSCASE)/buffer.c
SRC += $(SYSCASE)/parse_state.c
SRC += $(SYSCASE)/optee/smc/parse_handler.c
SRC += $(SYSCASE)/linux/smc/common.c
SRC += $(SYSCASE)/test_case.c
SRC += $(SYSCASE)/argument/number.c
SRC += $(SYSCASE)/argument/alloc.c
SRC += $(SYSCASE)/argument/buffer.c
SRC += $(SYSCASE)/argument/length.c
SRC += $(SYSCASE)/argument/vector_32.c
SRC += $(SYSCASE)/argument/vector_64.c
SRC += $(SYSCASE)/argument/reference.c
SRC += $(SYSCASE)/optee/smc/argument/msg.c
SRC += $(SYSCASE)/utils.c
SRC += smcchar_main.c

KDIR ?= /lib/modules/$(shell uname -r)/build/

$(MODULE_NAME)-objs = $(SRC:.c=.o)
obj-m += $(MODULE_NAME).o
# Compile with `-D SYSCASE_DUMMY` to disable AFL / SMC calls
EXTRA_CFLAGS=-I$(PWD)/include -I$(PWD)/$(SYSCASE)/include -std=gnu99 -D SYSCASE_SMC=y
ccflags-y += $(EXTRA_CFLAGS)

all:
	make -C $(KDIR) M=$(PWD) modules
	$(CC) -I include smcchar_test.c smc_call.c -o smcchar_test
clean:
	make -C $(KDIR) M=$(PWD) clean
	rm smcchar_test
