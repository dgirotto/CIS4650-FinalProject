/****************************************************/
/* File: tiny.l                                     */
/* Flex specification for TINY                       */
/****************************************************/

%{
#include "globals.h"
//#include "util.h"
#include "scan.h"

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
%}
number      	[0-9]+
word		[a-zA-Z]+
whitespace	[ \t]
%%

{number}        {return NUMBER;}
{word}    	{return WORD;}

"<"{word}">"	{return OPEN-TAG;}
"</"{word}">"	{return CLOSE-TAG;}

{word}"-"{word}	{return HYPHEN;}


//{newline}       { lineno++;}
{whitespace}    { /* skip whitespace */ }
.               { return ERROR; }

%%

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString, yytext, MAXTOKENLEN);
  return currentToken;
}

