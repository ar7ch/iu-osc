#!/bin/bash

# run as root
# shared libraries names on Debian 11, can vary on other distros

mkdir -p lofsdisk/{bin,tmp,lib,lib64} # create corresponding directories
echo "a" > lofsdisk/file1
echo "b" > lofsdisk/file2
cp /bin/{bash,ls,echo,pwd,sh} lofsdisk/bin # copy binaries
mkdir -p lofsdisk/lib/x86_64-linux-gnu
# next, copy shared libraries (very painful)
cp /lib/x86_64-linux-gnu/{libc.so.6,libpcre2-8.so.0,libselinux.so.1,libdl.so.2,libpthread.so.0,libtinfo.so.6} lofsdisk/lib/x86_64-linux-gnu/
cp -r lofsdisk/lib lofsdisk/lib64
cp /lib64/ld-linux-x86-64.so.2 lofsdisk/lib64
gcc ex2.c -o ex2.out 2> /dev/null # compile file
cp ex2.out lofsdisk/tmp # copy file
echo "RUNNING PROGRAM WITH CHROOT"
chroot lofsdisk /tmp/ex2.out # finally chroot and obtain result
echo "RUNNING PROGRAM WITHOUT CHROOT"
./ex2.out

# now for ex2.txt
echo "RUNNING PROGRAM WITH CHROOT" > ex2.txt
chroot lofsdisk /tmp/ex2.out >> ex2.txt # finally chroot and obtain result
echo "RUNNING PROGRAM WITHOUT CHROOT" >> ex2.txt
./ex2.out >> ex2.txt
