%{

#include "globals.h"
#include "util.h"
#include "scan.h"

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

%}

digit      	[0-9]
word		[a-zA-Z0-9]+

%%

"<"	{return LSQBRK;}
">"	{return RSQBRK;}

"-"?{digit}+ |				
"-"?{digit}*"."{digit}+ |
"-"?{digit}+"."{digit}* 		{return NUMBER;}

{word}    				{return WORD;}

"</"{word}">"				{return CLOSETAG;}

"<"{word}">"				{return OPENTAG;}

{word}"-"{word} |			
{word}"-"{word}"-"{word}		{return HYPHEN;}

{word}"'"{word} |
{word}"'"{word}"'"{word}		{return APOST;}

[ ]|"\n"|"\t"				{;}

.					{return PUNCT;}

<<EOF>>					{return(ENDFILE);} 

%%

TokenType getToken(void){
	static int firstTime = TRUE;
 	TokenType currentToken;

	if(firstTime){ 
		firstTime = FALSE;
    		yyin = source;
    		yyout = listing;
  	}

	currentToken = yylex();
	strncpy(tokenString, yytext, MAXTOKENLEN);
 	return currentToken;
}

