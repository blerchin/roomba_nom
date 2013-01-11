#!/usr/bin/env python
#coding=utf-8
import printer, Image, ImageDraw, RPi.GPIO as GPIO, re
import adc_photocell

WEBCAM_PATTERN="webcam_images/webcam*.jpg"

def get_image_file():
	recent=open('last_five.txt', 'r')
	print recent
	lastfile = ""
	for line in recent:
		lastfile = line
	
	return re.findall('(.*?)\n',lastfile)[0]

def print_job():
	p = printer.ThermalPrinter(serialport="/dev/ttyAMA0")
	i = Image.open(get_image_file())
	ratio = i.size[0] / i.size[1] 
	w = 384
	h = w/ratio
	i = i.resize((w, h ))
	i = i.convert('1')
	data = list(i.getdata())
	p.print_bitmap(data, w, h , False)
	p.linefeed()
	p.linefeed()
	p.linefeed()

while True:
	if adc_photocell.is_bright(25):
		print_job()

