#!/bin/bash
count=0
while : 
do
	fname="webcam$count.jpg"
	fswebcam -r 960x720 -d /dev/video0 $fname 
#keep a text file which always has names of last 5 saved images in order
	lines=$(wc -l < last_five.txt)
#tail returns 0 lines if less than -n value	
	if [ "$lines" -gt 5 ] 
	then	
		echo $lines	
		last=$(tail -n 5 last_five.txt)
		echo "$last" > last_five.txt
		wc -l < last_five.txt	
	fi
	echo "$fname" >> last_five.txt	
	if (( $count>5 ))
	then
		let count=0
	else
		let count=count+1
	fi
done
