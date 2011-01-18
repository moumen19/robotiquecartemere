
### General Makefile ###

# $(var)  replaced by the content of 'var'
# $@      name of the target
# $< 	  name of the first dependancy
# $^ 	  list of all the dependancies

#compilers commands
CC=gcc
CPP=g++
#compilation options
CFLAGS=-W -Wall -g
#linking options
LDFLAGS= -lcv -lhighgui
#-lcvaux -lcvcore



#program output name
EXEC=TP2

# main rule
all: $(EXEC)

# generates the executable from all .o list [Linker step]
$(EXEC): main.o colorSegmentation.o stereovision.o cameraCalibration.o
	$(CC) -o $@ $^ $(LDFLAGS)

# specify the dependancies; required with the next rule
#main.o: hello.h

# default rule: creates a .o for each .c [Compiler steps]
%.o: %.c %.h
	$(CC) -o $@ -c $< $(CFLAGS)

# remove every .o and executable program
clean:
	rm -fv *.o $(EXEC)

# clean then make
rebuild: clean all

#compression option
tar:
	tar cvzf test.tar.gz filesNames.c (or folderName)



### CodeBlocks Part ###
### This allows the makefile to be used by CodeBlocks ###
#####################################################################

# rules called by codeblocks
Release: all
	cp *.o ./obj/Release
	cp $(EXEC) ./bin/Release
	rm -fv *.o $(EXEC)
cleanRelease: clean
	cd ./obj/Release && rm -fv *.o
	cd ./bin/Release && rm -fv $(EXEC)
Debug: all
	cp *.o ./obj/Debug
	cp $(EXEC) ./bin/Debug
	rm -fv *.o $(EXEC)
cleanDebug: clean
	cd ./obj/Debug && rm -fv *.o
	cd ./bin/Debug && rm -fv $(EXEC)

#####################################################################

