all: handler

handler: Row.o Node.o
	gcc  handler.c Row.o Node.o

Row:
	gcc  -c Row.c

Node:
	gcc  -c Node.c

clean:
	rm a.out *.o
