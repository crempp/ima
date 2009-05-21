CC=g++
CFLAGS=-c -Wall `wx-config --cflags` -fno-strict-aliasing
LDFLAGS= `wx-config --libs`
VPATH=src:../include
SOURCES=canvas.cpp imaApp.cpp image.cpp ima_math.cpp mainframe.cpp gui.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ima

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	\rm *.o *~ ima

