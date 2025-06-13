my_program: main.o graph.o queue.o heap.o
	clang -o my_program main.o graph.o queue.o heap.o

main.o: main.c 
	clang -o main.o -c main.c

graph.o: graph.c
	clang -o graph.o -c graph.c

queue.o: queue.c
	clang -o queue.o -c queue.c

heap.o: heap.c
	clang -o heap.o -c heap.c

clean:
	rm *.o


run: my_program
	./my_program