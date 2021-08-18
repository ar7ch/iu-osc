#!/bin/bash
# list entries of /usr/bin that contain "gcc", sort these entries in reverse alphabetical order and save results in ex1.txt
ls /usr/bin | grep "gcc" | sort -d -r > ex1.txt
