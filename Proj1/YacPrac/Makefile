all: lex.yy.c y.tab.c
	gcc y.tab.c lex.yy.c -o comp
	
lex.yy.c: scan.l
	lex scan.l

y.tab.c: parse.y
	yacc -d parse.y

clean:
	rm comp lex.yy.c y.tab.c y.tab.h
