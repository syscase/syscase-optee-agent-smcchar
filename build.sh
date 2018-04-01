#!/usr/bin/env bash

SYSCASE=../syscase make KDIR=`pwd`/../../../linux ARCH=arm64 CROSS_COMPILE=`pwd`/../../../toolchains/aarch64/bin/aarch64-linux-gnu-
cp smcchar.ko `pwd`/../../../gen_rootfs/etc/smcchar.ko
