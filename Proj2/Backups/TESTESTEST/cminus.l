/*
File: cminus.l
Lex specification for C MINUS
*/

%{
#include "globals.h"
#include "util.h"
#include "scan.h"
#include "y.tab.h"

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
%}

digit           [0-9]
num             {digit}+
letter          [a-zA-Z]
id              {letter}+
newline         \n
whitespace      [ \t]+

%%

"if"            {return IF;}
"else"          {return ELSE;}
"int"           {return INT;}
"return"        {return RETURN;}
"void"          {return VOID;}
"while"         {return WHILE;}

"="             {return ASSIGN;}
"=="            {return EQ;}
"<"             {return LT;}
"<="            {return LTEQ;}
">"             {return GT;}
">="            {return GTEQ;}
"!="            {return NEQ;}

","             {return COMMA;}
"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}
";"             {return SEMI;}

"("             {return LPAREN;}
")"             {return RPAREN;}
"["             {return LSQ;}
"]"             {return RSQ;}
"{"             {return LCURLY;}
"}"             {return RCURLY;}

{num}           {return NUM;}
{id}            {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}

"/*"            {
                char c;
                while(1){
                        c = input();
                        if(c == '*'){
                                c = input();
                                if(c == '/'){
                                        break; /* end of comment; break out of while loop */
                                }
                        }
                        if(c == '\n'){
                                lineno++;
                        }
                        if(c == EOF){
                                break;
                        }
                }
                }
.               {return ERROR;}

%%

int getToken(void){
        static int firstTime = TRUE;
        int currentToken;

        if(firstTime){
                firstTime = FALSE;
                lineno++;
                yyin = source;
                yyout = listing;
        }

        currentToken = yylex();
        strncpy(tokenString, yytext, MAXTOKENLEN);

        if(TraceScan){
                fprintf(listing,"\t%d: ",lineno);
                printToken(currentToken,tokenString);
        }

        return currentToken;
}

