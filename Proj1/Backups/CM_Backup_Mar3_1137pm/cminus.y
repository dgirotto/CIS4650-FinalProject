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
 * compatible with the CMINUS scanner
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
			{
			   savedTree = $1; /* could also do: $$ = $1, then savedTree = $$ */
			}
			;

declaration-list	: declaration-list declaration 
			{
			   TreeNode * tree = $1; /* create a tree pointer; point to "declaration-list" (which stores the parse tree thus far) */
			   if(tree != NULL){ /* if the tree is not null ... */
			      while(tree->sibling != NULL){ /* find an empty (NULL) sibling */
			         tree = tree->sibling;
			      }
			      tree->sibling = $2; /* "declaration" is reduced and becomes the new sibling */
			      $$ = $1; /* "declaration-list" now stores/ references/ points to the modified tree (with the added sibling node) */
			   }
			   else{
			      $$ = $2; /* if the tree is NULL */
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
			{
			   TreeNode* t = $1;
			   if(t != NULL){
			      while(t != NULL){
			         t = t->sibling;
			      }
			      t->sibling = $2;
			      $$ = $1;
			   }
			   else{
			      $$ = $2;
			   }
			}
			| empty
			{
			   // do nothing ??? 
			}
			;

statement		: expression-stmt { $$ = $1; }
			| compound-stmt { $$ = $1; }
			| selection-stmt { $$ = $1; }
			| iteration-stmt { $$ = $1; }
			| return-stmt { $$ = $1; }
			;

expression-stmt		: expression SEMI
			{
			   $$ = $1; // is this right ??
			}
			| SEMI 
			{ 
			   // not sure what to do here
			}
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

simple-expression	: additive-expression LT additive-expression
			{
			   $$ = newExpNode(OpK);
			   $$->child[0] = $1;
			   $$->child[1] = $3;
			   $$->attr.op = LT; 
			}
			| additive-expression LTEQ additive-expression
			{
                           $$ = newExpNode(OpK);
                           $$->child[0] = $1;
                           $$->child[1] = $3;
                           $$->attr.op = LTEQ;

			}
                        | additive-expression GT additive-expression
                        {
                           $$ = newExpNode(OpK);
                           $$->child[0] = $1;
                           $$->child[1] = $3;
                           $$->attr.op = GT;

			}
                        | additive-expression GTEQ additive-expression
                        {
                           $$ = newExpNode(OpK);
                           $$->child[0] = $1;
                           $$->child[1] = $3;
                           $$->attr.op = GTEQ;
		
			}
                        | additive-expression DUBEQ additive-expression
                        {
                           $$ = newExpNode(OpK);
                           $$->child[0] = $1;
                           $$->child[1] = $3;
                           $$->attr.op = DUBEQ;

			}
                        | additive-expression NOTEQ additive-expression
                        {
                           $$ = newExpNode(OpK);
                           $$->child[0] = $1;
                           $$->child[1] = $3;
                           $$->attr.op = NOTEQ;
	
			}
			| additive-expression 
			{
                           // what to do here?
			   $$ = $1; // ??
			}
			;


additive-expression	: additive-expression PLUS term
			{
			   $$ = newExpNOde(OpK);
			   $$->child[0] = $1;
			   $$->child[1] = $3;
			   $$->attr.op = PLUS;
			}
			| additive-expression MINUS term
			{
			   $$ = newExpNode(OpK);
			   $$->child[0] = $1;
			   $$->child[1] = $3;
			   $$->attr.op = MINUS;
			}
			| term
			{
			   $$ = $1; // is this right?
			}
			;

term			: term TIMES factor 
			{
			   $$ = newExpNode(OpK);
			   $$->child[0] = $1;
			   $$->child[1] = $3;
			   $$->attr.op = TIMES;
			}
			| term OVER factor
			{
			   $$ = newExpNode(OpK);
			   $$->child[0] = $1;
			   $$->child[1] = $3;
			   $$->attr.op = OVER;
			}
			| factor
			{
			  $$ = $1; // is this right?
			}
			;

factor			: LBRACE expression RBRACE 
			{

			}
			| var
			{

			}
			| call
			{

			}
			| NUM	
			{

			}
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
			{
			   $$ = $1; // is this right?
			}
			;

empty			:
			{
				printf("Found an EMPTY statement\n");
			}
			;

