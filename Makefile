my_program: main.o graph.o
	clang -o my_program main.o graph.o

main.o: main.c
	clang -o main.o -c main.c

graph.o: graph.c
	clang -o graph.o -c graph.c



run: my_program
	./my_program	