#! /bin/bash

cd /dev/roomba_nom

if[ gpio -g read 17 ]
then
	nohup ./fswebcam.sh &
	sudo ./print_routine.py
	echo "starting roomba_nom"
else
	echo "short pin 17 to run roomba script at startup"
fi

