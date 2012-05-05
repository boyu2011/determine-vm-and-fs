
program:program.o
	g++ -o program program.o
program.o:program.cpp
	g++ -c -g program.cpp
clean:
	rm -f program.o program t*
