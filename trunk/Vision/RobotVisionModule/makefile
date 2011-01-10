#
# Bureau d'étude Robotique M2 ISEN 2010-2011
#
# DELBERGUE Julien
# JACQUEL Olivier
# PIETTE Ferdinand (ferdinand.piette@gmail.com)
#
# makefile

CC=g++
CFLAGS=-W -Wall -g
LDFLAGS= -lcv -lhighgui -lcvaux #-lserial -lboost_thread
EXEC=RobotVisionModule

all: $(EXEC)

$(EXEC): main.o AbstractVideoProcessingModule.o AbstractVideoSource.o Camera.o VideoFile.o Vision.o
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.cpp %.h
	$(CC) -o $@ -c $< $(CFLAGS)

#main.o: main.cpp
#	$(CC) $(CFLAGS) -c $< -o $@

#AbstractVideoProcessingModule.o: AbstractVideoProcessingModule.cpp AbstractVideoProcessingModule.h
#	$(CC) $(CFLAGS) -c $< -o $@

#AbstractVideoSource.o: AbstractVideoSource.cpp AbstractVideoSource.h
#	$(CC) $(CFLAGS) -c $< -o $@

#Camera.o: Camera.cpp Camera.h
#	$(CC) $(CFLAGS) -c $< -o $@

#VideoFile.o: VideoFile.cpp VideoFile.h
#	$(CC) $(CFLAGS) -c $< -o $@

#Vision.o: Vision.cpp Vision.h
#	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean rebuild $(EXEC)

clean:
	rm -f *.o $(EXEC)

rebuild: clean all


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

