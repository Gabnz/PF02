all: main

main: EntradaSalida.o
	g++ EntradaSalida.o -o main

EntradaSalida.o: EntradaSalida.cpp
	g++ -c EntradaSalida.cpp

clean:
	rm -rf *o main
