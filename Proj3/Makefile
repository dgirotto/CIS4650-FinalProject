CC = gcc
OBJS = y.tab.o lex.yy.o util.o main.o gencode.o

cminus: $(OBJS)
	$(CC) $(OBJS) -o cminus -ly -ll

main.o: main.c globals.h util.h scan.h parse.h
	$(CC) $(CFLAGS) -c main.c

gencode.o: gencode.c gencode.h globals.h y.tab.h
	$(CC) $(CFLAGS) -c gencode.c

util.o: util.c util.h globals.h y.tab.h
	$(CC) $(CFLAGS) -c util.c

lex.yy.c: cminus.l
	flex cminus.l

lex.yy.o: lex.yy.c globals.h util.h y.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c

y.tab.c: cminus.y
	yacc -d -v cminus.y

y.tab.o: y.tab.c globals.h util.h scan.h parse.h
	$(CC) $(CFLAGS) -c y.tab.c

clean:
	rm -f cminus $(OBJS) lex.yy.c y.tab.h y.tab.c y.output
