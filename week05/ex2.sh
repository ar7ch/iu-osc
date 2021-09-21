#!/bin/bash

# to see the results clearer, run two ex2.sh
# processes in different windows
# (see ex2_screenshot.png)

file=two.txt
lock=two.lock
maxlines=150
sleeptime=2

if [ -f $lock ] ; then sleep 0.5 ; fi

while [ 1 -eq 1 ]
do
	if [ ! -f $file ]
	then
		echo "file not found, creating"
		echo 0 > $file
	fi

	entry=0
	while [ -f $lock ]
	do
		if [ $entry -eq 0 ]
		then
			echo "busy waiting...":
			entry=1
		fi
	done
	if [ $entry -eq 1 ] ; then echo "busy waiting done" ; fi

	touch $lock
	if [ -f $lock ] ; then echo "created lock file" ; fi

	line=$(tail -n 1 $file)
	if [ $((line)) -ge $maxlines ] 
	then 
		echo "exceeded number of lines"
		rm -f $lock
		break
	fi
	tmp=$((line+1))
	echo "$tmp" >> $file
	echo "written $tmp to file"
	sleep $sleeptime
	rm -f $lock
	sleep $sleeptime
done

