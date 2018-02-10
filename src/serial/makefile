CC=$(CXX)


serial : main.o fonctions.o
	g++ -o serial main.o fonctions.o -lwiringPi -Wall

serial.o : main.cpp fonctions.cpp define.h
	g++ -o main.o fonctions.o -c main.cpp fonctions.cpp define.h -lwiringPi -Wall
