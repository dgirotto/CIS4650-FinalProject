CC = gcc
CFLAGS = 
OBJS = lex.yy.o util.o stack.o main.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o scanner -ll

main.o: main.c globals.h util.h scan.h stack.h
	$(CC) $(CFLAGS) -c main.c

stack.o:
	$(CC) $(CFLAGS) -c stack.c

util.o:
	$(CC) $(CFLAGS) -c util.c

lex.yy.c: scanner.l
	flex scanner.l

lex.yy.o: lex.yy.c globals.h util.h
	$(CC) $(CFLAGS) -c lex.yy.c

clean: 
	rm -f scanner $(OBJS) lex.yy.c



