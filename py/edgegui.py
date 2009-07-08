from Tkinter import *
import tkFileDialog
import PIL.Image, PIL.ImageTk
import os.path
import numpy
import im
import re
import threading

__author__ = 'Chad Rempp <crempp@gmail.com>'
__version__ = '0.0.1'
APP_NAME = 'Edge GUI'

# IsNum
# Checks if a string can be converted to an Int
def IsNum(num):
	mo = re.match(r'^-?(\d*\.\d\d|\d+)$',num)
	if mo and mo.group()==num: return 'yes'
	return 'no'

# Params
# A class that stores parameters for the whole program. Mostly related to the
# GUI
class Params:
	def __init__(self):
		self._values={}
		self._reset()
	def _reset(self):
		self._values['Background'] = '#161616'
		self._values['Font_c']     = ("Courier", 8)
		self._values['Forground']  = '#E7E9ED'
		self._values['Formats']    = [
			('Portable Network Graphics','*.png'),
			('JPEG / JFIF','*.jpg'),
			('Tiff','*.tiff'),
			('Windows Bitmap','*.bmp'),
			('CompuServer GIF','*.gif'),
			('All Files','*.*')]
		#self._values['C_Parms']   =	(font=self.parm._getitem('Font_c'),
		#		bg  =self.parm._getitem('Background'),
		#		fg  =self.parm._getitem('Forground'))
	def _getitem(self, akey):
		return self._values[akey]
	def _setitem(self, akey, value):
		self._values[akey] = value

# Imagedata
# the imagedata class. This stores the image array, undo array, PIL image and
# image parameters.
class Imagedata:
	def __init__(self):
		self._values={}
		self._reset()
		self.ar = numpy.zeros((2,2),dtype="uint8")

	def _reset(self):
		self._values['C_Dim_x']=832
		self._values['C_Dim_y']=640
		self._values['I_File']='none'
		self._values['I_Dim_x']=0
		self._values['I_Dim_y']=0

	def _getitem(self, akey):
		return self._values[akey]

	def _setitem(self, akey, value):
		self._values[akey] = value

	def loadimage(self,file_name):
		self.PIL_Image =PIL.Image.open(file_name)
		self.PIL_Undo = self.PIL_Image.copy()
		w,h = self.PIL_Image.size
		self._values['I_File'] = file_name
		self._values['I_Dim_x'] = w
		self._values['I_Dim_y'] = h
		self.ar = numpy.reshape(numpy.fromstring(self.PIL_Image.tostring(),dtype="uint8"),(h,w))
		self.edgedirection = numpy.zeros((h,w),dtype="float")

	def saveundo(self):
		self.PIL_Undo = self.PIL_Image.copy()

	def undo(self):
		if self._values['I_File'] != 'none':
			self.PIL_Image = self.PIL_Undo.copy()
			self.ar = numpy.reshape(numpy.fromstring(self.PIL_Image.tostring(),dtype="uint8"),
				(self._getitem('I_Dim_y'),self._getitem('I_Dim_x')))

class App:
	def __init__(self,parent):
		# Initialize helper classes
		self.parm = Params()
		self.imagedata = Imagedata()
		
		# Main frame container
		self.f_main = Frame(parent,bg=self.parm._getitem('Background'))
		self.f_main.pack()
		# Canvas
		self._create_canvas(self.f_main)
		# Controls
		self._create_controls(self.f_main)

	# _create_canvas
	def _create_canvas(self, parent):
		print "Creating canvas..."
		self.cv = Canvas(parent,width=self.imagedata._getitem('C_Dim_x'),height=self.imagedata._getitem('C_Dim_y'),bg='black')
		self.cv.pack(side=LEFT)
		print "Created canvas."

	# _create_controls
	# TODO - Figure out how to condense the parameters
	# 		http://www.python.org/doc/2.5.2/ref/function.html
	def _create_controls(self,parent):
		print "Creating controls..."
		# This is a dummy var for the radio buttons, should be able to do without this
		self.gradientvar = StringVar()
		self.gradientvar.set('rcr')
		
		# Control Frame
		self.f_controls = Frame(parent, bg=self.parm._getitem('Background'))
		self.f_controls.pack(side=TOP)
		# File name
		self.l_iname = Label(self.f_controls,
				text=self.imagedata._getitem('I_File'),
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'),
				height = 2, wraplength = 170)
		self.l_iname.grid(row=0,column=0,columnspan=4,sticky=W)
		# File Size
		self.l_isize = Label(self.f_controls,
				text=str(self.imagedata._getitem('I_Dim_x'))+"x"+str(self.imagedata._getitem('I_Dim_y')),
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'))
		self.l_isize.grid(row=1,column=0,columnspan=2,sticky=W)
		# Undo button
		Button(self.f_controls, text="Undo",command=self.undo,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'),
				padx=0,pady=0).grid(row=1,column=3,sticky=W)
		# Smoothing labels
		Label(self.f_controls, text="Smoothing________",
   				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground')).grid(row=2,column=0,columnspan=2,sticky=W)
		Label(self.f_controls, text="Wndw",
   				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground')).grid(row=2,column=2)
		Label(self.f_controls, text="s",
   				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground')).grid(row=2,column=3)
		# Averaging
		Button(self.f_controls, text="Avgage",command=self.run_average,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'),
				padx=0,pady=0).grid(row=3,column=0,columnspan=2,sticky=W)
		self.e_avgwndw = Entry(self.f_controls, width=3,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'))
		self.e_avgwndw.grid(row=3,column=2,sticky=W)
		self.e_avgwndw.insert(0,'3')
		# Gaussian
		Button(self.f_controls, text="Gauss Smooth",command=self.run_gausssmooth,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'),
				padx=0,pady=0).grid(row=4,column=0,columnspan=2,sticky=W)
		self.e_gausswndw = Entry(self.f_controls, width=3,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'))
		self.e_gausswndw.grid(row=4,column=2,sticky=W)
		self.e_gausswndw.insert(0,'2')
		self.e_gaussconst = Entry(self.f_controls, width=4,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'))
		self.e_gaussconst.grid(row=4,column=3,sticky=W)
		self.e_gaussconst.insert(0,'1.0')
		# Gradient label
   		Label(self.f_controls, text="Gradient Method_______",
   				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground')).grid(row=20,column=0,columnspan=3,sticky=W)
		# Gradient threshold label
		Label(self.f_controls, text="Thrsh",
   				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground')).grid(row=20,column=3,sticky=W)
		# Gradient rcr button
		Radiobutton(self.f_controls, text="Roberts Cross", value='rcr', variable=self.gradientvar,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground')).grid(row=21,column=0,columnspan=3,sticky=W)
		# Gradient rcr entry
		self.e_rcrthresh = Entry(self.f_controls, width=4,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'))
		self.e_rcrthresh.grid(row=21,column=3,sticky=W)
		self.e_rcrthresh.insert(0,'0')
		# Gradient pwt button
		Radiobutton(self.f_controls, text="Prewitt", value='pwt', variable=self.gradientvar,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground')).grid(row=22,column=0,columnspan=3,sticky=W)
		# Gradient pwt entry
		self.e_pwtthresh = Entry(self.f_controls, width=4,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'))
		self.e_pwtthresh.grid(row=22,column=3,sticky=W)
		self.e_pwtthresh.insert(0,'0')
		# Gradient sob button
		Radiobutton(self.f_controls, text="Sobel", value='sob', variable=self.gradientvar,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground')).grid(row=23,column=0,columnspan=3,sticky=W)
		# Gradient sob entry
		self.e_sobthresh = Entry(self.f_controls, width=4,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'))
		self.e_sobthresh.grid(row=23,column=3,sticky=W)
		self.e_sobthresh.insert(0,'0')
		# Gradient sch button
		Radiobutton(self.f_controls, text="Scharr", value='sch', variable=self.gradientvar,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground')).grid(row=24,column=0,columnspan=3,sticky=W)
		# Gradient sch entry
		self.e_schthresh = Entry(self.f_controls, width=4,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'))
		self.e_schthresh.grid(row=24,column=3,sticky=W)
		self.e_schthresh.insert(0,'0')
		# Gradient lap button
		Radiobutton(self.f_controls, text="Laplace", value='lap', variable=self.gradientvar,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground')).grid(row=25,column=0,columnspan=3,sticky=W)
		# Gradient lap entry
		self.e_lapthresh = Entry(self.f_controls, width=4,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'))
		self.e_lapthresh.grid(row=25,column=3,sticky=W)
		self.e_lapthresh.insert(0,'0')
		# Gradient kir button
		Radiobutton(self.f_controls, text="Kirsch", value='kir', variable=self.gradientvar,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground')).grid(row=26,column=0,columnspan=3,sticky=W)
		# Gradient kir entry
		self.e_kirthresh = Entry(self.f_controls, width=4,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'))
		self.e_kirthresh.grid(row=26,column=3,sticky=W)
		self.e_kirthresh.insert(0,'0')
		# Gradient button
		Button(self.f_controls, text="Run Gradient",command=self.run_gradient,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'),width=27,padx=0,pady=0).grid(row=27,column=0,columnspan=4,sticky=W)
		# Edge detection
		Label(self.f_controls, text="Edge Detection______________",
   				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground')).grid(row=28,column=0,columnspan=4,sticky=W)
		Button(self.f_controls, text="Canny",command=self.run_canny,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'),padx=0,pady=0).grid(row=29,column=0,sticky=W)
		Button(self.f_controls, text="Crack",command=self.run_crack,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'),padx=0,pady=0).grid(row=29,column=1,sticky=W)
		# File label
		Label(self.f_controls, text="File Operations_____________",
   				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground')).grid(row=40,column=0,columnspan=4,sticky=W)
		# Open button
		Button(self.f_controls, text="Open",command=self.load_image,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'),width=27,padx=0,pady=0).grid(row=41,column=0,columnspan=4,sticky=W)
		# Save button
		Button(self.f_controls, text="Save",command=self.save_image,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'),width=27,padx=0,pady=0).grid(row=42,column=0,columnspan=4,sticky=W)
		# Exit button
		Button(self.f_controls, text="Exit", command=self.f_main.quit,
				font=self.parm._getitem('Font_c'),
				bg  =self.parm._getitem('Background'),
				fg  =self.parm._getitem('Forground'),width=27,padx=0,pady=0).grid(row=43,column=0,columnspan=4,sticky=W)
		print "Created controls."

	# load_image
	def load_image(self):
		file_name = tkFileDialog.askopenfilename(filetypes = self.parm._getitem('Formats'),
				title = "Open Image", initialdir = "./images")
		if file_name != None:
			self.imagedata.loadimage(file_name)
			self.l_iname['text'] = self.imagedata._getitem('I_File')
			self.l_isize['text'] = str(self.imagedata._getitem('I_Dim_x'))\
				+"x"+str(self.imagedata._getitem('I_Dim_y'))
			# Put the image on the canvas
			self.refresh()

	# save_image
	def save_image(self):
		file_name = tkFileDialog.asksaveasfilename(filetypes = self.parm._getitem('Formats'),
				title = "Open Image", initialdir = "./images")
		if file_name != None:
			self.imagedata.PIL_Image.save(file_name, "PNG")
			self.l_iname['text'] = self.imagedata._getitem('I_File')
			self.l_isize['text'] = str(self.imagedata._getitem('I_Dim_x'))\
				+"x"+str(self.imagedata._getitem('I_Dim_y'))

	# refresh
	def refresh(self):
		print "  Refreshing"
		# Clear canvas
		self.cv.delete(ALL)
		# Convert array to PIL image
		self.imagedata.PIL_Image = PIL.Image.fromstring('L',(self.imagedata._getitem('I_Dim_x'),self.imagedata._getitem('I_Dim_y')),self.imagedata.ar.tostring())
		# Convert PIL image to Tk image
		self.im = PIL.ImageTk.PhotoImage(self.imagedata.PIL_Image)
		# put Tk image on the canvas
		self.cv.create_image(self.imagedata._getitem('C_Dim_x')/2,
			self.imagedata._getitem('C_Dim_y')/2,
			image=self.im)

	# undo
	def undo(self):
		self.imagedata.undo()
		self.refresh()

	# run_average
	# Averaging smooth
	def run_average(self):
		print "  Running averaging filter"
		if (self.imagedata._getitem('I_File') == 'none'):
			print "ERROR: No image loaded"
		elif (IsNum(self.e_avgwndw.get()) and int(self.e_avgwndw.get())>=3 and (int(self.e_avgwndw.get())-1)%2==0):
			self.imagedata.ar = im.average(self.imagedata.ar,int(self.e_avgwndw.get()))
			self.refresh()
		else:
			print "ERROR: Invalid window size"

	# run_gausssmooth
	# Averaging smooth
	def run_gausssmooth(self):
		if (self.imagedata._getitem('I_File') == 'none'):
			print "ERROR: No image loaded"
		elif (IsNum(self.e_gausswndw.get()) and IsNum(self.e_gaussconst.get()) and\
				int(self.e_gausswndw.get()) >= 0 and int(self.e_gausswndw.get()) < 255 and\
				float(self.e_gaussconst.get()) > 0 and float(self.e_gaussconst.get()) <= 10):
			print "  Running gaussian smooth"
			ran = 0
			c_image = self.imagedata.ar.copy()
			im.gausssmooth(self.imagedata.ar, c_image, ran, int(self.e_gausswndw.get()), float(self.e_gaussconst.get()))
			self.imagedata.ar = c_image.copy()
			self.refresh()
		else:
			print "ERROR: Window or s is out of bounds or NaN."

	# run_gradient
	def run_gradient(self):
		# Save undo
		self.imagedata.saveundo()
		if (self.imagedata._getitem('I_File') == 'none'):
			print "ERROR: No image loaded"

		# Gradient - Roberts Cross
		elif (self.gradientvar.get() == 'rcr'):
			print "  Running Roberts Cross operator"
			if (IsNum(self.e_rcrthresh.get()) and\
					int(self.e_rcrthresh.get()) >= 0 and\
					int(self.e_rcrthresh.get()) < 255 ):
				r,c=numpy.shape(self.imagedata.ar)
				ran = range(0,r)
				part = im.partition(ran,im.num_threads)
				thresh = int(self.e_rcrthresh.get())
				c_image=numpy.zeros((r,c),dtype="uint8")
				thread_list = []
				# Create and start threads
				for i in range(0,im.num_threads):
					print "  Starting thread " + str(i)
					thrd = im.ImThread (im.robertscross, (self.imagedata.ar, c_image, part[i], thresh))
					thread_list.append(thrd)
					thrd.start()
				# Join threads
				for thrd in thread_list:
					thrd.join()
				self.imagedata.ar = c_image.copy()
				self.refresh()
				
			else:
				print "ERROR: Threshhold out of bounds or NaN."

		# Gradient - Prewitt
		elif (self.gradientvar.get() == 'pwt'):
			print "  Running Prewitt operator"
			if (IsNum(self.e_pwtthresh.get()) and\
					int(self.e_pwtthresh.get()) >= 0 and\
					int(self.e_pwtthresh.get()) < 255 ):
				r,c=numpy.shape(self.imagedata.ar)
				ran = range(0,r)
				part = im.partition(ran,im.num_threads)
				thresh = int(self.e_pwtthresh.get())
				c_image=numpy.zeros((r,c),dtype="uint8")
				thread_list = []
				# Create and start threads
				for i in range(0,im.num_threads):
					print "  Starting thread " + str(i)
					thrd = im.ImThread (im.prewitt, (self.imagedata.ar, c_image, part[i], thresh))
					thread_list.append(thrd)
					thrd.start()
				# Join threads
				for thrd in thread_list:
					thrd.join()
				self.imagedata.ar = c_image.copy()
				self.refresh()
			else:
				print "ERROR: Threshhold out of bounds or NaN."

		# Gradient - Sobel
		elif (self.gradientvar.get() == 'sob'):
			print "  Running Sobel operator"
			if (IsNum(self.e_sobthresh.get()) and\
					int(self.e_sobthresh.get()) >= 0 and\
					int(self.e_sobthresh.get()) < 255 ):
				r,c=numpy.shape(self.imagedata.ar)
				ran = range(0,r)
				part = im.partition(ran,im.num_threads)
				thresh = int(self.e_sobthresh.get())
				c_image=numpy.zeros((r,c),dtype="uint8")
				thread_list = []
				# Create and start threads
				for i in range(0,im.num_threads):
					print "  Starting thread " + str(i)
					thrd = im.ImThread (im.sobel, (self.imagedata.ar, c_image, self.imagedata.edgedirection, part[i], thresh))
					thread_list.append(thrd)
					thrd.start()
				# Join threads
				for thrd in thread_list:
					thrd.join()
				self.imagedata.ar = c_image.copy()
				self.refresh()
			else:
				print "ERROR: Threshhold out of bounds or NaN."

		# Gradient - Scharr
		elif (self.gradientvar.get() == 'sch'):
			print "  Running Scharr operator"
			if (IsNum(self.e_schthresh.get()) and\
					int(self.e_schthresh.get()) >= 0 and\
					int(self.e_schthresh.get()) < 255 ):
				r,c=numpy.shape(self.imagedata.ar)
				ran = range(0,r)
				part = im.partition(ran,im.num_threads)
				thresh = int(self.e_schthresh.get())
				c_image=numpy.zeros((r,c),dtype="uint8")
				thread_list = []
				# Create and start threads
				for i in range(0,im.num_threads):
					print "  Starting thread " + str(i)
					thrd = im.ImThread (im.scharr, (self.imagedata.ar, c_image, part[i], thresh))
					thread_list.append(thrd)
					thrd.start()
				# Join threads
				for thrd in thread_list:
					thrd.join()
				self.imagedata.ar = c_image.copy()
				self.refresh()
			else:
				print "ERROR: Threshhold out of bounds or NaN."

		# Gradient - Laplacian
		elif (self.gradientvar.get() == 'lap'):
			print "  Running Laplacian operator"
			if (IsNum(self.e_lapthresh.get()) and\
					int(self.e_lapthresh.get()) >= 0 and\
					int(self.e_lapthresh.get()) < 255 ):
				r,c=numpy.shape(self.imagedata.ar)
				ran = range(0,r)
				part = im.partition(ran,im.num_threads)
				thresh = int(self.e_lapthresh.get())
				c_image=numpy.zeros((r,c),dtype="uint8")
				thread_list = []
				# Create and start threads
				for i in range(0,im.num_threads):
					print "  Starting thread " + str(i)
					thrd = im.ImThread (im.laplacian, (self.imagedata.ar, c_image, part[i], thresh))
					thread_list.append(thrd)
					thrd.start()
				# Join threads
				for thrd in thread_list:
					thrd.join()
				self.imagedata.ar = c_image.copy()
				self.refresh()
			else:
				print "ERROR: Threshhold out of bounds or NaN."

		# Gradient - Kirsch
		elif (self.gradientvar.get() == 'kir'):
			print "  Running Kirsch operator"
			if (IsNum(self.e_kirthresh.get()) and\
					int(self.e_kirthresh.get()) >= 0 and\
					int(self.e_kirthresh.get()) < 255 ):
				r,c=numpy.shape(self.imagedata.ar)
				ran = range(0,r)
				part = im.partition(ran,im.num_threads)
				thresh = int(self.e_kirthresh.get())
				c_image=numpy.zeros((r,c),dtype="uint8")
				thread_list = []
				# Create and start threads
				for i in range(0,im.num_threads):
					print "  Starting thread " + str(i)
					thrd = im.ImThread (im.kirsch, (self.imagedata.ar, c_image, part[i], thresh))
					thread_list.append(thrd)
					thrd.start()
				# Join threads to main program
				for thrd in thread_list:
					thrd.join()
				self.imagedata.ar = c_image.copy()
				self.refresh()	
			else:
				print "ERROR: Threshhold out of bounds or NaN."
		else:
			print "ERROR: Couldn't run operator"

	# run_canny
	def run_canny(self):
		print "Canny"
		im.canny((self.imagedata.edgedirection, self.imagedata.ar))
		self.refresh()

	# run_crack
	def run_crack(self):
		print "Crack"

# Main program
if __name__ == "__main__":
	print "Starting..."
	root = Tk()
	root.title('EDGUI %s' % __version__)
	app = App(root)
	print "Started."
	root.mainloop()


