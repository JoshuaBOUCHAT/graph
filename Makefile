my_program: main.o graph.o queue.o
	clang -o my_program main.o graph.o queue.o

main.o: main.c 
	clang -o main.o -c main.c

graph.o: graph.c 
	clang -o graph.o -c graph.c

queue.o: queue.c
	clang -o queue.o -c queue.c


run: my_program
	./my_program