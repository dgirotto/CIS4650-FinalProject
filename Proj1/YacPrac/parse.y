%{
//this gets prepended to the output file (comes first)

#include <stdio.h>
#include <stdlib.h>

extern int yylex(); // yylex() is the lexical analyzer associated with our lex generator
void yyerror(char*);
%}

%union{
	int i;
}

%type <i> E F
%token <i> NUM
%token PLUS

%%

S	: E 
	{
	   printf("DONE PARSING. THE ANSWER IS: %d\n",$1);
	}
	;
E	: E PLUS F
	{
	   $$ = $1 + $3;
	}
	| F
	{}
	;
	
F 	: NUM
	{
	   printf("Found a NUM: %d\n",$1);
	   $$ = $1;
	}
	;
%%

//the below gets appended to the output file

void yyerror(char * msg){ // gets called whenever there is an error
	fprintf(stderr,"%s\n",msg);
	exit(1);
}

int main() {
	yyparse(); // the parser

return 0;
}
