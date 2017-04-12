/*
	File: cminus.y
	The CMINUS Yacc/Bison spec file 
*/

#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *

static char * savedName; /* for use in assignments */
static int savedLineNo;  /* ditto */
static TreeNode * savedTree; /* stores syntax tree for later return */

extern int yychar;

int yyerror(char * message){
	fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
	fprintf(listing,"Current token: ");
	printToken(yychar,tokenString);
	return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with the TINY scanner
 */
static int yylex(void){
	return getToken();
}

TreeNode * parse(void){ 
	yyparse();
	return savedTree;
}

%}

%token IF ELSE INT RETURN VOID WHILE
%token INPUT OUTPUT
%token EQ DUBEQ LT LTEQ GT GTEQ NOTEQ
%token PLUS MINUS TIMES OVER
%token LPAREN RPAREN LSQBRK RSQBRK LBRACE RBRACE COMMA SEMI
%token ID NUM
%token ERROR

%nonassoc EQ DUBEQ LT LTEQ GT GTEQ NOTEQ 
%left PLUS MINUS
%left TIMES OVER

%% 

/* GRAMMAR FOR CMINUS */

program			: declaration-list
			{savedTree = $1;}
			;

declaration-list	: declaration-list declaration 
			{
			YYSTYPE tree = $1;
			if(tree != NULL){
				while(tree->sibling != NULL){
					tree = tree->sibling;
				}
				tree->sibling = $2;
				$$ = $1;
			}
			else{
				$$ = $3;
			}
			}
			| declaration
			{
			$$ = $1;
			}
			;

declaration		: var-declariation
			{}
			| fun-declaration
			{}
			;

var-declaration		: type-specifier ID SEMI
			{}
			| type-specifier ID LSQBRK NUM RSQBRK SEMI
			{}
			;

type-specifier		: INT
			{}
			| VOID
			{}
			;

fun-declaration 	: type-specifier ID LBRACE params RBRACE compound-stmt
			{}
			;

params			: param-list
			{}
			| VOID
			{}
			;

param-list		: param-list COMMA param
			{}
			| param
			{}
			;

param			: type-specifier ID 
			{}
			| type-specifier ID LSQBRK RSQBRK
			{}
			;

compound-stmt		: LPAREN local-declarations statement-list RPAREN
			{}
			;

local-declarations	: local-declarations var-declaration 
			{}
			| empty
			{}
			;

statement-list		: statement-list statement
			{}
			| empty
			{}
			;

statement		: expression-stmt
			{}
			| compound-stmt
			{}
			| selection-stmt
			{}
			| iteration-stmt
			{}
			| return-stmt
			{}
			;

expression-stmt		: expression SEMI
			{}
			| SEMI
			{}
			;

selection-stmt		: IF LBRACE expression RBRACE statement
			{}
			| IF LBRACE expression RBRACE statement ELSE statement
			{}
			;

iteration-stmt		: WHILE LBRACE expression RBRACE statement
			{}
			;

return-stmt 		: RETURN SEMI
			{}
			| RETURN expression SEMI
			{}
			;

expression		: var EQ expression 
			{}
			| simple-expression 
			{}
			;

var			: ID
			{}
			| ID LSQBRK expression RSQBRK
			{}
			;

simple-expression	: additive-expression relop additive-expression
			{}
			| additive-expression 
			{}
			;

relop			: LTEQ
			{}
			| LT
			{}
			| GTEQ
			{}
			| GT
			{}
			| DUBEQ
			{}
			| NOTEQ
			{}
 			;


additive-expression	: additive-expression addop term
			{}
			| term
			{}
			;

addop			: PLUS
			{}
			| MINUS
			{}
			;
 
term			: term mulop factor 
			{}
			| factor
			{}
			;

mulop			: TIMES
			{}
			| OVER 
			{}
			;

factor			: LBRACE expression RBRACE 
			{}
			| var
			{}
			| call
			{}
			| NUM	
			{}
			;

call			: ID LBRACE args RBRACE
			{}
			;

args			: arg-list 
			{}
			| empty	
			{}
			;

arg-list		: arg-list COMMA expression
			{}
			| expression 
			{}
			;

empty			:
			{
				printf("Found an EMPTY statement\n");
			}
			;

