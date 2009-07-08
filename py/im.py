# Linear Algebra
#    matrixprod(A,B)
#
# Filters
#    lpf(image,window_radius):
#    hpf(image,window_radius):
#
# Edge Detection Operators
#    robertscross(image,thresh):
#    sobel
#    scharr
#    prewitt
#    laplace
#    kirsch
#
# Edge Linking
#    Hough
#
# Other
#    fft
#    ifft

import numpy
import threading

#num_threads should be a power of 2
num_threads = 4

# ImThread
# This is the tread class which calls the functions as threads.
class ImThread(threading.Thread):
	def __init__(self, func, params):
		print "    initializing thread"
		self.func = func
		self.params = params
		threading.Thread.__init__(self)
	def run(self):
		print "    Running thread"
		self.func(self.params)

# partition
# Partitions the group of items in $group into a number of parts given by $parts
# It does this by selecting every $parts item
def partition(group,parts):
	partition = []
	for i in range(0,parts):
		part = []
		for j in group:
			if (j % parts) == i:
				part.append(j)
		partition.append(part)
	return partition

# convolve
# Sum of elementwise products. c = Sum(a_ij * b_ij)
def convolve(A,B):
	result = 0
	if numpy.shape(A) != numpy.shape(B):
		print "Error: shape mismatch in convolve"
	else:
		result = 0
		for i in range(0,numpy.shape(A)[0]):
			for j in range(0,numpy.shape(A)[1]):
				result += A[i,j]*B[i,j]
	return result

# Averaging smooth
def average(image, window_radius):
	r,c=numpy.shape(image)
	s_image=numpy.zeros((r,c),dtype="uint8")
	thresh=0.1111
	# Loop through image pixels
	for i in range(1, r-1):
		for j in range(1, c-1):
			p = int(image[i-1,j-1])+int(image[i-1,j])\
			+int(image[i-1,j+1])+int(image[i,j-1])\
			+int(image[i,j])+int(image[i,j+1])\
			+int(image[i+1,j-1])+int(image[i+1,j])\
			+int(image[i+1,j-1])
			s_image[i,j] = int(thresh*p)
	return s_image

# The Gaussian smooth
# TODO: Use ints rather than floats, thread and use the seprability property
def gausssmooth(image_in, image_out, ran, window_radius, s):
	print "    Gauss Smoothing"
	rows,columns = numpy.shape(image_in)
	# Define gaussian
	def G(x,y,s):
		return(numpy.power(numpy.e,-((numpy.square(x)+numpy.square(y))/(2*numpy.square(s)))))/(2*numpy.pi*numpy.square(s))
	# Create Gaussian kernel
	kern = numpy.zeros((2*window_radius+1,2*window_radius+1),dtype="float")
	for i in range(-window_radius,window_radius+1):
		for j in range(-window_radius,window_radius+1):
			#print "(" + str(i) + "," + str(j) + ")"
			kern[i+window_radius,j+window_radius]=G(i,j,s)
	print "    Kernel,"
	print kern
	# Convolve kernel with image
	for i in range(window_radius,rows-window_radius):
		for j in range(window_radius,columns-window_radius):
			image_out[i,j] = convolve(kern,image_in[i-window_radius:i+window_radius+1,j-window_radius:j+window_radius+1])
	print "    Smoothed"
			
			

# The Roberts Cross operator (threaded)
# Takes an image and returns the edge gradient image
# http://en.wikipedia.org/wiki/Roberts_Cross
def robertscross((image_in, image_out, ran, thresh)):
	print "    In Roberts"
	rows,columns = numpy.shape(image_in)
	del ran[-1]
	if (thresh == 0): # not thresholded
		for i in ran:
			for j in range(0, columns-1):
				image_out[i,j] = (abs(int(image_in[i,j])\
					-int(image_in[i+1,j+1]))\
					+abs(int(image_in[i+1,j])\
					-int(image_in[i,j+1])))
	else: # thresholded
		for i in ran:
			for j in range(0, columns-1):
				p = (abs(int(image_in[i,j])\
					-int(image_in[i+1,j+1]))\
					+abs(int(image_in[i+1,j])\
					-int(image_in[i,j+1])))
				if (p > thresh):
					image_out[i,j] = 255
				else:
					image_out[i,j] = 0
	print "    Finished Roberts"

# The Prewitt operator (threaded)
# Takes an image and returns the edge gradient image
# http://en.wikipedia.org/wiki/Prewitt
def prewitt((image_in, image_out, ran, thresh)):
	print "    In Prewitt"
	rows,columns = numpy.shape(image_in)
	del ran[-1]
	if (thresh == 0): # not thresholded
		for i in ran:
			for j in range(0, columns-1):
				Gx = abs(int(image_in[i-1,j+1])\
					+int(image_in[i,j+1])\
					+int(image_in[i+1,j+1])\
					-int(image_in[i-1,j-1])\
					-int(image_in[i,j-1])\
					-int(image_in[i+1,j-1]))
				Gy = abs(int(image_in[i+1,j-1])\
					+int(image_in[i,j])\
					+int(image_in[i+1,j+1]))\
					-int(image_in[i-1,j-1])\
					-int(image_in[i-1,j])\
					-int(image_in[i-1,j+1])
				image_out[i,j] = max(Gx,Gy)
	else: # thresholded
		for i in ran:
			for j in range(0, columns-1):
				Gx = abs(int(image_in[i-1,j+1])\
					+int(image_in[i,j+1])\
					+int(image_in[i+1,j+1])\
					-int(image_in[i-1,j-1])\
					-int(image_in[i,j-1])\
					-int(image_in[i+1,j-1]))
				Gy = abs(int(image_in[i+1,j-1])\
					+int(image_in[i,j])\
					+int(image_in[i+1,j+1]))\
					-int(image_in[i-1,j-1])\
					-int(image_in[i-1,j])\
					-int(image_in[i-1,j+1])
				p = max(Gx,Gy)
				if (p > thresh):
					image_out[i,j] = 255
				else:
					image_out[i,j] = 0
	print "    Finished Prewitt"

# The Sobel operator (threaded, records direction)
# Takes an image and returns the edge gradient image
# http://en.wikipedia.org/wiki/Sobel_operator
def sobel((image_in, image_out, image_dir, ran, thresh)):
	print "    In Sobel"
	rows,columns = numpy.shape(image_in)
	del ran[-1]
	if (thresh == 0): # not thresholded
		for i in ran:
			for j in range(0, columns-1):
				Gx = abs(int(image_in[i-1,j-1])\
					+2*int(image_in[i,j-1])\
					+int(image_in[i+1,j-1])\
					-int(image_in[i-1,j+1])\
					-2*int(image_in[i,j+1])\
					-int(image_in[i+1,j+1]))
				Gy = abs(int(image_in[i-1,j-1])\
					+2*int(image_in[i-1,j])\
					+int(image_in[i-1,j+1])\
					-int(image_in[i+1,j-1])\
					-2*int(image_in[i,j])\
					-int(image_in[i+1,j+1]))
				image_out[i,j] = max(Gx,Gy)
				if (Gx != 0):
					image_dir[i,j] = numpy.arctan(Gy/Gx)
				else:
					image_dir[i,j] = 0
	else: # thresholded
		for i in ran:
			for j in range(0, columns-1):
				Gx = abs(int(image[i-1,j-1])\
					+2*int(image[i,j-1])\
					+int(image[i+1,j-1])\
					-int(image[i-1,j+1])\
					-2*int(image[i,j+1])\
					-int(image[i+1,j+1]))
				Gy = abs(int(image[i-1,j-1])\
					+2*int(image[i-1,j])\
					+int(image[i-1,j+1])\
					-int(image[i+1,j-1])\
					-2*int(image[i,j])\
					-int(image[i+1,j+1]))
				p = max(Gx,Gy)
				if (Gx != 0):
					image_dir[i,j] = numpy.arctan(Gy/Gx)
				else:
					image_dir[i,j] = 0

				if (p > thresh):
					image_out[i,j] = 255
				else:
					c_image_out[i,j] = 0
	print "    Finished Sobel"

# The Scharr operator (threaded)
# Takes an image and returns the edge gradient image
# http://en.wikipedia.org/wiki/Sobel_operator#Scharr_Operator
def scharr((image_in, image_out, ran, thresh)):
	print "    In Scharr"
	rows,columns = numpy.shape(image_in)
	del ran[-1]
	if (thresh == 0): # not thresholded
		for i in ran:
			for j in range(0, columns-1):
				Gx = abs(3*int(image_in[i-1,j-1])\
					+10*int(image_in[i,j-1])\
					+3*int(image_in[i+1,j-1])\
					-3*int(image_in[i-1,j+1])\
					-10*int(image_in[i,j+1])\
					-3*int(image_in[i+1,j+1]))
				Gy = abs(3*int(image_in[i-1,j-1])\
					+10*int(image_in[i-1,j])\
					+3*int(image_in[i-1,j+1])\
					-3*int(image_in[i+1,j-1])\
					-10*int(image_in[i+1,j])\
					-3*int(image_in[i+1,j+1]))
				image_out[i,j] = max(Gx,Gy)
	else: # thresholded
		for i in ran:
			for j in range(0, columns-1):
				Gx = abs(3*int(image_in[i-1,j-1])\
					+10*int(image_in[i,j-1])\
					+3*int(image_in[i+1,j-1])\
					-3*int(image_in[i-1,j+1])\
					-10*int(image_in[i,j+1])\
					-3*int(image_in[i+1,j+1]))
				Gy = abs(3*int(image_in[i-1,j-1])\
					+10*int(image_in[i-1,j])\
					+3*int(image_in[i-1,j+1])\
					-3*int(image_in[i+1,j-1])\
					-10*int(image_in[i+1,j])\
					-3*int(image_in[i+1,j+1]))
				p = max(Gx,Gy)
				if (p > thresh):
					image_out[i,j] = 255
				else:
					image_out[i,j] = 0
	print "    Finished Scharr"

# The Laplacian operator (threaded)
# Takes an image and returns the edge gradient image
# http://en.wikipedia.org/wiki/Laplacian
def laplacian((image_in, image_out, ran, thresh)):
	print "    In Laplacian"
	rows,columns = numpy.shape(image_in)
	del ran[-1]
	if (thresh == 0): # not thresholded
		for i in ran:
			for j in range(0, columns-1):
				p = abs(4*int(image_in[i,j])\
					-int(image_in[i-1,j])\
					-int(image_in[i,j+1])\
					-int(image_in[i+1,j])\
					-int(image_in[i,j-1]))
				image_out[i,j] = p
	else: # thresholded
		for i in ran:
			for j in range(0, columns-1):
				p = abs(4*int(image_in[i,j])\
					-int(image_in[i-1,j])\
					-int(image_in[i,j+1])\
					-int(image_in[i+1,j])\
					-int(image_in[i,j-1]))
				if (p > thresh):
					image_out[i,j] = 255
				else:
					image_out[i,j] = 0
	print "    Finished Laplacian"

# The Kirsch operator (threaded)
# Takes an image and returns the edge gradient image
# http://de.wikipedia.org/wiki/Kirsch-Operator
def kirsch((image_in, image_out, ran, thresh)):
	print "    In Kirsch"
	rows,columns = numpy.shape(image_in)
	del ran[-1]
	if (thresh == 0): # not thresholded
		del ran[-1]
		for i in ran:
			for j in range(0, columns-1):
				G1=abs(5*int(image_in[i-1,j-1])\
					+5*int(image_in[i-1,j])\
					+5*int(image_in[i-1,j+1])\
					-3*int(image_in[i,j+1])\
					-3*int(image_in[i,j-1])\
					-3*int(image_in[i+1,j-1])\
					-3*int(image_in[i+1,j])\
					-3*int(image_in[i+1,j+1]))
				G2=abs(-3*int(image_in[i-1,j-1])\
					+5*int(image_in[i-1,j])\
					+5*int(image_in[i-1,j+1])\
					+5*int(image_in[i,j+1])\
					-3*int(image_in[i+1,j+1])\
					-3*int(image_in[i+1,j])\
					-3*int(image_in[i+1,j-1])\
					-3*int(image_in[i,j-1]))
				G3=abs(-3*int(image_in[i-1,j-1])\
					-3*int(image_in[i-1,j])\
					+5*int(image_in[i-1,j+1])\
					+5*int(image_in[i,j+1])\
					+5*int(image_in[i+1,j+1])\
					-3*int(image_in[i+1,j])\
					-3*int(image_in[i+1,j-1])\
					-3*int(image_in[i,j-1]))
				G4=abs(-3*int(image_in[i-1,j-1])\
					-3*int(image_in[i-1,j])\
					-3*int(image_in[i-1,j+1])\
					+5*int(image_in[i,j+1])\
					+5*int(image_in[i+1,j+1])\
					+5*int(image_in[i+1,j])\
					-3*int(image_in[i+1,j-1])\
					-3*int(image_in[i,j-1]))
				G5=abs(-3*int(image_in[i-1,j-1])\
					-3*int(image_in[i-1,j])\
					-3*int(image_in[i-1,j+1])\
					-3*int(image_in[i,j+1])\
					+5*int(image_in[i+1,j+1])\
					+5*int(image_in[i+1,j])\
					+5*int(image_in[i+1,j-1])\
					-3*int(image_in[i,j-1]))
				G6=abs(-3*int(image_in[i-1,j-1])\
					-3*int(image_in[i-1,j])\
					-3*int(image_in[i-1,j+1])\
					-3*int(image_in[i,j+1])\
					-3*int(image_in[i+1,j+1])\
					+5*int(image_in[i+1,j])\
					+5*int(image_in[i+1,j-1])\
					+5*int(image_in[i,j-1]))
				G7=abs(+5*int(image_in[i-1,j-1])\
					-3*int(image_in[i-1,j])\
					-3*int(image_in[i-1,j+1])\
					-3*int(image_in[i,j+1])\
					-3*int(image_in[i+1,j+1])\
					-3*int(image_in[i+1,j])\
					+5*int(image_in[i+1,j-1])\
					+5*int(image_in[i,j-1]))
				G8=abs(+5*int(image_in[i-1,j-1])\
					+5*int(image_in[i-1,j])\
					-3*int(image_in[i-1,j+1])\
					-3*int(image_in[i,j+1])\
					-3*int(image_in[i+1,j+1])\
					-3*int(image_in[i+1,j])\
					-3*int(image_in[i+1,j-1])\
					+5*int(image_in[i,j-1]))
				image_out[i,j]=max(G1,G2,G3,G4,G5,G6,G7,G8)
	else: # thresholded
		for i in ran:
			for j in range(0, columns-1):
				G1=abs(5*int(image_in[i-1,j-1])\
					+5*int(image_in[i-1,j])\
					+5*int(image_in[i-1,j+1])\
					-3*int(image_in[i,j+1])\
					-3*int(image_in[i,j-1])\
					-3*int(image_in[i+1,j-1])\
					-3*int(image_in[i+1,j])\
					-3*int(image_in[i+1,j+1]))
				G2=abs(5*int(image_in[i-1,j-1])\
					+5*int(image_in[i-1,j])\
					-3*int(image_in[i-1,j+1])\
					+5*int(image_in[i,j+1])\
					-3*int(image_in[i,j-1])\
					-3*int(image_in[i+1,j-1])\
					-3*int(image_in[i+1,j])\
					-3*int(image_in[i+1,j+1]))
				G3=abs(5*int(image_in[i-1,j-1])\
					-3*int(image_in[i-1,j])\
					-3*int(image_in[i-1,j+1])\
					+5*int(image_in[i,j+1])\
					-3*int(image_in[i,j-1])\
					+5*int(image_in[i+1,j-1])\
					-3*int(image_in[i+1,j])\
					-3*int(image_in[i+1,j+1]))
				G4=abs(-3*int(image_in[i-1,j-1])\
					-3*int(image_in[i-1,j])\
					-3*int(image_in[i-1,j+1])\
					+5*int(image_in[i,j+1])\
					-3*int(image_in[i,j-1])\
					+5*int(image_in[i+1,j-1])\
					+5*int(image_in[i+1,j])\
					-3*int(image_in[i+1,j+1]))
				p=max(G1,G2,G3,G4)
				if (p>thresh):
					image_out[i,j] = 255
				else:
					image_out[i,j] = 0
	print "    Finished Kirsch"

# The range of angles go from 0 to pi. They should go from -pi/2 to pi/2 since that the range of arctan
# http://www.pages.drexel.edu/~weg22/can_tut.html
def canny((image_dir, image_out)):
	print "    Canny"
	#image_dir = image_in.copy()
	rows,columns = numpy.shape(image_dir)
	for i in range(0,rows):
		for j in range(0,columns):
			
			if ((image_dir[i,j] >= 0) and (image_dir[i,j] < 0.3927)) or\
			((image_dir[i,j] >= 2.7489) and (image_dir[i,j] < 3.1416)):
				image_out[i,j] = 0
			elif (image_dir[i,j] >= 0.3927) and (image_dir[i,j] < 1.1781):
				image_out[i,j] = 45
			elif (image_dir[i,j] >= 1.1781) and (image_dir[i,j] < 1.9635):
				image_out[i,j] = 90
			elif (image_dir[i,j] >= 1.9635) and (image_dir[i,j] < 2.7489):
				image_out[i,j] = 135
			'''
			if ((image_dir[i,j] <= 1.5708) and (image_dir[i,j] > 1.1781)) or\
			((image_dir[i,j] >= -1.5708) and (image_dir[i,j] <= -1.1781)):
				image_out[i,j] = 90
			elif (image_dir[i,j] <= 1.1781) and (image_dir[i,j] > 0.3927):
				image_out[i,j] = 45
			elif (image_dir[i,j] <= 0.3927) and (image_dir[i,j] > -0.3927):
				image_out[i,j] = 0
			elif (image_dir[i,j] <= -0.3927) and (image_dir[i,j] > -1.1781):
				image_out[i,j] = 135
			'''
			print str(image_dir[i,j]) + " " + str(image_out[i,j])
	# Non-maximum suppression
	
			
	print "    Canny finished"







