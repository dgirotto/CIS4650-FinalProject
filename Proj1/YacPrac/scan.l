%{

#include <stdio.h>
#include <stdlib.h>

#include "y.tab.h"
#include "scan.h"

%}

%%

"+"		return PLUS;
[a-z]+		return ID;
[0-9]+		{
		   yylval.i = atoi(yytext); 
		   return NUM;
		}

[ \t\n]		; 
.		printf("ERR\n");

%%

int yywrap(void){
	return 1;
}

