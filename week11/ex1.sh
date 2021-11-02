#!/bin/bash

# execute as root

if [ $(whoami) != "root" ]; then 
	echo "please run this script as root"
	exit 1
fi

export J=$(pwd)/lofsdisk

fallocate -l 51M lofs.img # create empty file of 51M size
ls -lh lofs.img # check this file
losetup -f lofs.img # set lofs.img on available loopback device
mkfs.ext4 lofs.img # create filesystem on file
mkdir -p lofsdisk
mount -t ext4 lofs.img lofsdisk # mount file
echo "image created and mounted"


