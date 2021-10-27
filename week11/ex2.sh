#!/bin/bash

# run as root
# shared libraries names on Debian 11, can vary on other distros

mkdir lofsdisk/{bin,tmp,lib,lib64} # create corresponding directories
echo "a" > lofsdisk/file1
echo "b" > lofsdisk/file2
cp /bin/{bash,ls,echo,pwd,sh} lofsdisk/bin # copy binaries
mkdir lofsdisk/lib/x86_64-linux-gnu
# next, copy shared libraries (very painful)
cp /lib/x86_64-linux-gnu{libc.so.6,libpcre2-8.so.0,libselinux.so.1,libdl.so.2,libpthread.so.0,libtinfo.so.6} lofsdisk/lib/x86_64-linux-gnu
cp -r lofsdisk/lib lofsdisk/lib64
cp /lib64/ld-linux-x86-64.so.2 lofsdisk/lib64
gcc ex2.c -o lofsdisk/tmp/ex2.out # compile file
chroot lofsdisk /tmp/ex2.out # finally chroot and obtain result
