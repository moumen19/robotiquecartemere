all: main.o Debug.o BusRS232.o BusI2C.o Communication.o Constraint.o Data.o DataFusion.o Planning.o Robot.o Strategy.o
	g++ -Wall main.o Debug.o BusRS232.o BusI2C.o Communication.o Constraint.o Data.o DataFusion.o Planning.o Robot.o Strategy.o -lpthread -o robot_prgm

main.o: main.cpp
	g++ -Wall -c main.cpp -o main.o

Debug.o: Debug.cpp Debug.hpp
	g++ -Wall -c Debug.cpp -o Debug.o

BusRS232.o: ./robot/BusRS232.cpp ./robot/BusRS232.hpp
	g++ -Wall -c ./robot/BusRS232.cpp -o BusRS232.o

BusI2C.o: ./robot/BusI2C.cpp ./robot/BusI2C.hpp
	g++ -Wall -c ./robot/BusI2C.cpp -o BusI2C.o

Communication.o: ./robot/Communication.cpp ./robot/Communication.hpp
	g++ -Wall -c ./robot/Communication.cpp -o Communication.o

Constraint.o: ./robot/Constraint.cpp ./robot/Constraint.hpp
	g++ -Wall -c ./robot/Constraint.cpp -o Constraint.o

Data.o: ./robot/Data.cpp ./robot/Data.hpp
	g++ -Wall -c ./robot/Data.cpp -o Data.o

DataFusion.o: ./robot/DataFusion.cpp ./robot/DataFusion.hpp
	g++ -Wall -c ./robot/DataFusion.cpp -o DataFusion.o

Planning.o: ./robot/Planning.cpp ./robot/Planning.hpp
	g++ -Wall -c ./robot/Planning.cpp -o Planning.o

Robot.o: ./robot/Robot.cpp ./robot/Robot.hpp
	g++ -Wall -c ./robot/Robot.cpp -o Robot.o

Strategy.o: ./robot/Strategy.cpp ./robot/Strategy.hpp
	g++ -Wall -c ./robot/Strategy.cpp -o Strategy.o

clean:
	rm -f *.o robot_prgm

rebuild: clean all
