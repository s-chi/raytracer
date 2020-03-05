CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
CFLAGS = -g -DOSX
INCFLAGS = -I./glm-0.9.2.7 -I/usr/X11/include -I./include/
LDFLAGS =  -L./osxlib/ -lm -lstdc++ -lfreeimage
else
CFLAGS = -O3 -march=native -fopenmp #-g #-Wall -pg
INCFLAGS = -I./glm-0.9.2.7 -I./include/ -I/usr/X11R6/include -I/sw/include
LDFLAGS = -L/usr/X11R6/lib -lX11 -lfreeimage
endif

RM = /bin/rm -f 
all: raytrace
raytrace: main.o readfile.o Transform.o raycast.o gridcube.o variables.h Transform.h readfile.h basic.h raycast.h gridcube.h
	$(CC) $(CFLAGS) -o raytrace main.o readfile.o Transform.o raycast.o gridcube.o $(INCFLAGS) $(LDFLAGS) 
main.o: main.cpp variables.h Transform.h readfile.h basic.h raycast.h gridcube.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
readfile.o: readfile.cpp variables.h Transform.h readfile.h basic.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c readfile.cpp
Transform.o: Transform.cpp basic.h Transform.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Transform.cpp
raycast.o: raycast.cpp basic.h variables.h gridcube.h raycast.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c raycast.cpp
gridcube.o: gridcube.cpp basic.h variables.h gridcube.h raycast.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c gridcube.cpp
clean: 
	$(RM) *.o raytrace *.png


 
