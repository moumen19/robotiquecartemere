CC=g++ 
CFLAGS=-W -Wall 
LDFLAGS= -lserial -lboost_thread 
EXEC=robot_prgm 


all: $(EXEC)

$(EXEC): main.o Debug.o Buffer.o BusRS232.o Communication.o Constraint.o Data.o DataFusion.o Planning.o Robot.o RS232Asservissement.o Strategy.o
	$(CC) $^ $(LDFLAGS) -o $@

main.o: main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

Debug.o: Debug.cpp Debug.hpp
	$(CC) $(CFLAGS) -c $< -o $@

Buffer.o: ./robot/Buffer.cpp ./robot/Buffer.hpp 
	$(CC) $(CFLAGS) -c $< -o $@

BusRS232.o: ./robot/BusRS232.cpp ./robot/BusRS232.hpp
	$(CC) $(CFLAGS) -c $< -o $@

Communication.o: ./robot/Communication.cpp ./robot/Communication.hpp
	$(CC) $(CFLAGS) -c $< -o $@

Constraint.o: ./robot/Constraint.cpp ./robot/Constraint.hpp
	$(CC) $(CFLAGS) -c $< -o $@

Data.o: ./robot/Data.cpp ./robot/Data.hpp
	$(CC) $(CFLAGS) -c $< -o $@

DataFusion.o: ./robot/DataFusion.cpp ./robot/DataFusion.hpp
	$(CC) $(CFLAGS) -c $< -o $@

Planning.o: ./robot/Planning.cpp ./robot/Planning.hpp
	$(CC) $(CFLAGS) -c $< -o $@

Robot.o: ./robot/Robot.cpp ./robot/Robot.hpp
	$(CC) $(CFLAGS) -c $< -o $@

RS232Asservissement.o: ./robot/RS232Asservissement.cpp ./robot/RS232Asservissement.hpp
	$(CC) $(CFLAGS) -c $< -o $@

Strategy.o: ./robot/Strategy.cpp ./robot/Strategy.hpp
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean rebuild $(EXEC)

clean:
	rm -f *.o $(EXEC)

rebuild: clean all

