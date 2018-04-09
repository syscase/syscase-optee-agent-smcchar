#!/usr/bin/env bash

root=$(dirname "$(readlink -f "$0")")
rootfs="$root/../../../gen_rootfs"
SYSCASE=../syscase make KDIR="$root/../../../linux" ARCH=arm64 CROSS_COMPILE="$root/../../../toolchains/aarch64/bin/aarch64-linux-gnu-"
cp smcchar.ko "$rootfs/etc/smcchar.ko"

filelist="$rootfs/filelist.txt"
if [ "$(cat "$filelist" | grep smcchar.ko)" == "" ] ; then
  echo "Adding smcchar.ko to 'gen_rootfs/filelist.txt'"
  echo "file /etc/smcchar.ko etc/smcchar.ko 644 0 0" >> "$filelist"
fi
