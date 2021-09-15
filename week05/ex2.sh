file=two.txt
lock=two.lock
maxlines=150
#for c in {1..$maxlines}
while [ 1 -eq 1 ]
do
#if [ $(wc -l $file) -ge $maxlines ]
#then
#	echo "file full"
#	break
#fi

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
	rm -f $lock
	ln $file $lock
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
	sleep 5
	rm -f $lock
done
