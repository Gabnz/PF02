all: main

main: main.o Simulacion.o
	g++ main.o Simulacion.o -o main

main.o: main.cpp
	g++ -c main.cpp

Simulacion.o: Simulacion.cpp Simulacion.h
	g++ -c Simulacion.cpp

clean:
	rm -rf *.o main
