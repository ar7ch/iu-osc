#!/bin/bash

# run as root
if [ $(whoami) != "root" ]; then 
	echo "please run this script as root"
	exit 1
fi

export LODIR=$(pwd)/lofsdisk

function import_so () {
	list="$(ldd /bin/{bash,cat,echo,ls,sh} | egrep -o '/lib.*\.[0-9]')"
	for i in $list;
	do 
		dest=$(echo $i | cut -d'/' -f2-)
		cp  -vn "$i" $LODIR/$dest
	done
}


mkdir -p $LODIR/{bin,tmp,lib,lib64} # create corresponding directories
mkdir -p $LODIR/lib/x86_64-linux-gnu
echo "a" > $LODIR/file1
echo "b" > $LODIR/file2
cp /bin/{bash,ls,echo,pwd,sh} $LODIR/bin # copy binaries
# next, copy shared libraries (very painful)
# shared libraries names on Debian 11, can vary on other distros
#cp /lib/x86_64-linux-gnu/{libc.so.6,libpcre2-8.so.0,libselinux.so.1,libdl.so.2,libpthread.so.0,libtinfo.so.6} lofsdisk/lib/x86_64-linux-gnu/
# replaced hardcoded version with some smart copying
import_so
gcc ex2.c -o ex2.out 2> /dev/null # compile file
cp ex2.out $LODIR/tmp # copy file
echo "RUNNING PROGRAM WITH CHROOT"
chroot $LODIR /tmp/ex2.out # finally chroot and obtain result
echo "RUNNING PROGRAM WITHOUT CHROOT"
./ex2.out

# now for ex2.txt
echo "RUNNING PROGRAM WITH CHROOT" > ex2.txt
chroot lofsdisk /tmp/ex2.out >> ex2.txt # finally chroot and obtain result
echo "RUNNING PROGRAM WITHOUT CHROOT" >> ex2.txt
./ex2.out >> ex2.txt
