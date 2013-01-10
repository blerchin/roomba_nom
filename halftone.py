import ImageDraw, ImageStat

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


