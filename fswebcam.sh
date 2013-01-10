#!/bin/bash
count=0
while : 
do
	fname="webcam$count.jpg"
	fswebcam -r 960x720 -d /dev/video0 $fname 
	
	if (( $count>5 ))
	then
		let count=0
	else
		let count=count+1
	fi
done
