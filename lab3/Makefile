all: zad1 licz test_time test_stack test_heap

zad1: zad1.o
	gcc -o zad1 zad1.o -lm
zad1.o: zad1.c
	gcc -c zad1.c
	
licz: licz.o
	gcc -o licz licz.o -lm

licz.o: licz.c
	gcc -c licz.c
	
test_time: test_time.o
	gcc -o test_time test_time.o -lm

test_time.o: test_time.c
	gcc -c test_time.c
	
test_stack: test_stack.o
	gcc -o test_stack test_stack.o -lm

test_stack.o: test_stack.c
	gcc -c test_stack.c
	
test_heap: test_heap.o
	gcc -o test_heap test_heap.o -lm

test_heap.o: test_heap.c
	gcc -c test_heap.c


