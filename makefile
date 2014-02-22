all: main2

main2: main2.o Simulacion.o
	g++ main2.o Simulacion.o -o main2

main2.o: main2.cpp
	g++ -c main2.cpp

Simulacion.o: Simulacion.cpp Simulacion.h
	g++ -c Simulacion.cpp

clean:
	rm -rf *.o main2
