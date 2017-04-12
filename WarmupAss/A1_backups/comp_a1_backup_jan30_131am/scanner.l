%{

#include "globals.h"
#include "util.h"
#include "scan.h"

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

%}

digit      	[0-9]
word		[a-zA-Z0-9]+
newline		[\n]
%%

"<"	{return LSQBRK;}
">"	{return RSQBRK;}
"/"	{return FSLASH;}
"="	{return EQ;}
["]	{return QUOTE;}



"-"?{digit}+ |				
"-"?{digit}*"."{digit}+ |
"-"?{digit}+"."{digit}* |		{return NUMBER;}

{word}    				{return WORD;}

"</"{word}">"				{return CLOSETAG;}

{word}"-"{word} |			
{word}"-"{word}"-"{word}		{return HYPHEN;}

{word}"'"{word} |
{word}"'"{word}"'"{word}		{return APOST;}






{newline}       { lineno++;}
.               { return ERROR; }

<<EOF>> 	{ printf("End of file reached!\n");
		  return(ENDFILE); 
		}

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

