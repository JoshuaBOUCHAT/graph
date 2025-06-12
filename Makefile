main.o: main.c
	clang -o main.o -c main.c

graph.o: graph.c
	clang -o graph.o -c graph.c

my_program: main.o
	clang -o my_program main.o

run: my_program
	./my_program	