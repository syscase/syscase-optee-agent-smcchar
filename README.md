# smcchar
## Build

### x64 debug build
To debug the module it is possible to build the module without ARM64 inline assembly
by setting `SYSCASE_DUMMY=1`:

```bash
$ SYSCASE_DUMMY=1 SYSCASE=../syscase make 
```

### OPTEE cross compile build

To cross compile the module for ARM64 we can use the OPTEE aarch64 toolchain in
`optee/toolchains/aarch64`:

```bash
$ SYSCASE=../syscase make KDIR=`pwd`/../../../linux ARCH=arm64 CROSS_COMPILE=`pwd`/../../../toolchains/aarch64/bin/aarch64-linux-gnu-
```

## Loading the module in OPTEE

Start OPTEE and execute following commands to load `smcchar.ko`:

```bash
# mound_shared /mnt
# insmod /mnt/optee_examples/agent/smcchar/smcchar.ko
```
