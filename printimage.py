#!/usr/bin/env python
#coding=utf-8
import printer, Image, ImageDraw, ImageStat, RPi.GPIO as GPIO

#halftone code based on StackOverflow post by fraxel
def halftone(im, sample, scale):
	bw = im.convert('L')
	dots = []
	angle = 0
	bw = bw.rotate(angle, expand=1)
	size = (int(bw.size[0]*scale), int(bw.size[1]*scale))
	print str(size)
	half_tone = Image.new('L', size)
	draw = ImageDraw.Draw(half_tone)
	for x in xrange(0, bw.size[0], sample):
		for y in xrange(0, bw.size[1], sample):
			box = bw.crop((x, y, x+sample, y+sample))
			stat = ImageStat.Stat(box)
			diameter = (stat.mean[0] / 255) ** 0.5
			edge = 0.5*(1-diameter)
			x_pos, y_pos = (x+edge)*scale, (y+edge)*scale
			box_edge = sample*diameter*scale
			draw.ellipse((x_pos, y_pos, x_pos + box_edge, y_pos + box_edge),  fill=255)
	half_tone = half_tone.rotate(-angle, expand=1)
	width_half, height_half = half_tone.size
	xx=(width_half-im.size[0]*scale) / 2
	yy=(height_half-im.size[1]*scale) / 2
	half_tone = half_tone.crop((xx, yy, xx+ im.size[0]*scale, yy + im.size[1]*scale))
	dots.append(half_tone)
	angle += 15
	
	ht_image = Image.merge('L', dots)
	return ht_image


def print_job():
	p = printer.ThermalPrinter(serialport="/dev/ttyAMA0")
	i = Image.open("thumbs_up_large.png")
	bb = i.getbbox()
	ratio = bb[2] / bb[3]
	w = 384
	h = w/ratio
	i = i.resize(( w, h ))
	i = halftone(i, 5, 1)
	
	data = list(i.getdata())
	p.print_bitmap(data, w, h , False)
	p.linefeed()
	p.linefeed()
	p.linefeed()
GPIO.setmode(GPIO.BCM)
BUTTON =25 
GPIO.setup(BUTTON, GPIO.IN)

while True:
	if GPIO.input(BUTTON):
		print "Pressed!" 
		print_job()


