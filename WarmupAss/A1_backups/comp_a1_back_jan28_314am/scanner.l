%{

#include "globals.h"
#include "util.h"
#include "scan.h"

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

%}

number      	[0-9]+
word		[0-9a-zA-Z]+
whitespace	[ \t]
newline		[\n]
%%

{number}        {return NUMBER;}
{word}    	{return WORD;}

"<"{word}">"	{return OPENTAG;}
"</"{word}">"	{return CLOSETAG;}

{word}"-"{word}	{return HYPHEN;}


{newline}       { lineno++;}
{whitespace}    { /* skip whitespace */ }
.               { return ERROR; }

%%
//C CODE

TokenType getToken(void){
	static int firstTime = TRUE;
 	TokenType currentToken;

	if(firstTime){ 
		firstTime = FALSE;
    		lineno++;
    		yyin = source;
    		yyout = listing;
  	}

	currentToken = yylex();
	strncpy(tokenString, yytext, MAXTOKENLEN);
 	return currentToken;
}

