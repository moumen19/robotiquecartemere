#
# Bureau d'étude Robotique M2 ISEN 2010-2011
#
# DELBERGUE Julien
# JACQUEL Olivier
# PIETTE Ferdinand (ferdinand.piette@gmail.com)
#
# makefile

CC=gcc
CPP=g++
CFLAGS=-W -Wall -g
LDFLAGS= -lcxcore -lcv -lhighgui -lcvaux
#LDFLAGS= -lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_video -lopencv_features2d -lopencv_ml -lopencv_highgui -lopencv_objdetect -lopencv_contrib -lopencv_legacy
#-lserial -lboost_thread
EXEC=RobotVisionModule

all: $(EXEC)

#$(EXEC): main.o Stereovision.o Camera.o GenericImageProcessing.o colorSegmentation.o
$(EXEC): colorSegmentation.o main.o
	$(CC) $^ $(LDFLAGS) -o $@

colorSegmentation.o: colorSegmentation.c colorSegmentation.h
	$(CC) -o $@ -c $< $(CFLAGS)

main.o: main.cpp
	$(CC) -o $@ -c $< $(CFLAGS)



#%.o: %.cpp %.hpp
#	$(CC) -o $@ -c $< $(CFLAGS)


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

