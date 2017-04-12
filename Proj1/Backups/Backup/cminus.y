/*
	File: cminus.y
	The CMINUS Yacc/Bison spec file 
*/

%{

#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *

static char * savedName;
static int savedLineNo;
static int savedNum;

static YYSTYPE savedTree; /* stores syntax tree for later return */

extern int yychar;

int yyerror(char * message){
	fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
	fprintf(listing,"Current token: ");
	printToken(yychar,tokenString);
	return 0;
}

static int yylex(void){
	return getToken();
}

TreeNode * parse(void){ 
	yyparse();
	return savedTree;
}

%}

%token IF ELSE INT RETURN VOID WHILE
%token ASSIGN EQ LT LTEQ GT GTEQ NEQ
%token PLUS MINUS TIMES OVER
%token LPAREN RPAREN LSQ RSQ LCURLY RCURLY COMMA SEMI
%token ID NUM
%token ERROR

%nonassoc EQ LT LTEQ GT GTEQ NEQ
%left PLUS MINUS
%left TIMES OVER

%% 

/* GRAMMAR FOR CMINUS */

program			: declaration_list
			{
			   savedTree = $1;
			}			
			;

declaration_list	: declaration_list declaration 
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
			      $$ = $2;
			   }
			}			
			| declaration { $$ = $1; } 
			;

declaration		: var_declaration { $$ = $1; } 
			| fun_declaration { $$ = $1; }
			;

var_declaration		: type_specifier ID 
			{
			   printf("b. ID = %s\n",tokenString);
			   savedName = copyString(tokenString); //store the ID
			   savedLineNo = lineno;
			}
			SEMI
			{
			   $$ = newDecNode(VarK);
                           $$->child[0] = $1;
                           $$->attr.name = savedName; //store the ID
			   $$->lineno = savedLineNo;
			}
			| type_specifier ID LSQ NUM RSQ SEMI
			;

type_specifier		: INT
			{

			} 
			| VOID 
			{

			}
			;

fun_declaration 	: type_specifier ID 
			{
			   printf("in fun_dec. ID = %s\n",tokenString);
			   savedName = copyString(tokenString);
			   savedLineNo = lineno;	

			}
			LPAREN params RPAREN compound_stmt
			{
			   $$ = newDecNode(FunK);
			   $$->child[0] = $1;
			   $$->child[1] = $5;
			   $$->child[2] = $7;
			   $$->attr.name = savedName;
			   $$->lineno = lineno;
			}
			;

params			: param_list
			| VOID
			;

param_list		: param_list COMMA param
			| param
			;

param			: type_specifier ID 
			| type_specifier ID LSQ RSQ
			;

compound_stmt		: LCURLY local_declarations statement_list RCURLY
			{
			   
			}
			;

local_declarations	: /* empty */ 
			| local_declarations var_declaration
			{
			   
			}
			;

statement_list		: /* empty */
			| statement_list statement
			;

statement		: expression_stmt
			| compound_stmt
			| selection_stmt
			| iteration_stmt
			| return_stmt
			;

expression_stmt		: expression SEMI
			| SEMI 
			;

selection_stmt		: IF LPAREN expression RPAREN statement
			| IF LPAREN expression RPAREN statement ELSE statement
			;

iteration_stmt		: WHILE LPAREN expression RPAREN statement
			;

return_stmt 		: RETURN SEMI
			| RETURN expression SEMI
			;

expression		: var ASSIGN expression 
			| simple_expression
			;

var			: ID
			| ID LSQ expression RSQ
			;

simple_expression	: additive_expression LT additive_expression
			| additive_expression LTEQ additive_expression
                        | additive_expression GT additive_expression
                        | additive_expression GTEQ additive_expression
                        | additive_expression EQ additive_expression
                        | additive_expression NEQ additive_expression
			| additive_expression 
			;


additive_expression	: additive_expression PLUS term
			| additive_expression MINUS term
			| term
			;

term			: term TIMES factor 
			| term OVER factor
			| factor
			;

factor			: LPAREN expression RPAREN
			| var 
			| call
			| NUM	
			;

call			: ID LPAREN args RPAREN
			;

args			: /* empty */
			| arg_list 
			;

arg_list		: arg_list COMMA expression
			| expression 
			;
