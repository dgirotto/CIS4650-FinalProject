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
%token ASSIGN EQ LT LTEQ GT GTEQ NOTEQ
%token PLUS MINUS TIMES OVER
%token LPAREN RPAREN LSQ RSQ LCURLY RCURLY COMMA SEMI
%token ID NUM
%token ERROR

%nonassoc LT LTEQ GT GTEQ NOTEQ
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
			      while(tree->sibling != NULL){ /* find an empty (NULL) sibling to store the next declaration */
			         tree = tree->sibling;
			      }
			      tree->sibling = $2; /* "declaration" is reduced and becomes the new sibling */
			      $$ = $1; /* "declaration-list" now stores/ references/ points to the modified tree (with the added sibling node) */
			   }
			   else{  // the tree is currently empty
			      $$ = $2; /* store the tree's first node */
			   }
			}
			;

declaration		: var-declaration { $$ = $1; }
			| fun-declaration { $$ = $1; }
			;

var-declaration		: type-specifier ID SEMI
			{
			   $$ = newDecNode(VarK);
			   $$->child[0] = $1;
			   $$->attr.name = copyString(tokenString); //store the ID

			}
			| type-specifier ID LSQ NUM RSQ SEMI
			{
			   $$ = newDecNode(VarK);
			   $$->child[0] = $1;
			   $$->attr.name = copyString(tokenString);
			   // NEED TO STORE BOTH ID AND NUM ... ???
 
			}
			;

type-specifier		: INT { $$ = $1; }
			| VOID { $$ = $1; }
			;

fun-declaration 	: type-specifier ID LPAREN params RPAREN compound-stmt
			{
			   $$ = newDecNode(FunK);
			   $$->child[0] = $1;
			   $$->child[1] = $4;
			   $$->child[2] = $6;
			   $$->attr.name = copyString(tokenString);
			}
			;

params			: param-list
			{
			   $$ = $1;
			}
			| VOID
			{
			   $$ = $1;
			}
			;

param-list		: param-list COMMA param
			{
			   // ???
			}
			| param
			{
			   $$ = $1;
			}
			;

param			: type-specifier ID 
			{
			   $$ = newDecNode(ParamK);
			   $$->child[0] = $1;
			   $$->attr.name = copyString(tokenString);
			}
			| type-specifier ID LSQ RSQ
			{
			   $$ = newDecNode(ParamK);
			   $$->child[0] = type-specifier;
			   $$->attr.name = copyString(tokenString);
			}
			;

compound-stmt		: LCURLY local-declarations statement-list RCURLY
			{
			   $$ = newStmtNode(CmpdK);
			   $$->child[0] = $2;
			   $$->child[1] = $3;
			   
			}
			;

local-declarations	: local-declarations var-declaration 
			{
			// ????
			}
			| empty
			{
			   $$ = NULL; // is this right??
			}
			;

statement-list		: statement-list statement
			{


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
			   $$ = NULL;
			   // not sure what to do here
			}
			;

selection-stmt		: IF LPAREN expression RPAREN statement
			{
			   $$ = newStmtNode(IfK);
			   $$->child[0] = $3;
			   $$->child[1] = $5;
			}
			| IF LPAREN expression RPAREN statement ELSE statement
			{
			   $$ = newStmtNode(IfK);
			   $$->child[0] = $3;
			   $$->child[1] = $5;
			   $$->child[2] = $7;
			}
			;

iteration-stmt		: WHILE LBRACE expression RBRACE statement
			{
			   $$ = newStmtNode(WhileK);
			   $$->child[0] = $3;
			   $$->child[1] = $5;
			}
			;

return-stmt 		: RETURN SEMI
			{
			   // should still create a return node??
			   $$ = newStmtNode(ReturnK);
			   // ???
			}
			| RETURN expression SEMI
			{
			   $$ = newStmtNode(ReturnK);
			   $$->child[0] = $2;
			}
			;

expression		: var ASSIGN expression 
			{
			   // is this an expression or statement? 
			   $$ = newStmtNode(AssignK); // is this correct?
			   $$->child[0] = $1;
			   $$->child[1] = $3;

			}
			| simple-expression { $$ = $1; }
			;

var			: ID
			{
			   // what is savedName and savedLineNo ???
			   $$ = newExpNode(IdK);
			   $$->attr.name = copyString(tokenString); // implement copyString() ... what/where is "tokenString" ???	
			}
			| ID LSQBRK expression RSQBRK
			   // not sure if this is correct  (probably not correct ) ...
			   $$ = newExpNode(IdK);
			   $$->attr.name = copyString(tokenString);
			   $$->child[0] = $3;

			{
			   
			}
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
                        | additive-expression EQ additive-expression
                        {
                           $$ = newExpNode(OpK);
                           $$->child[0] = $1;
                           $$->child[1] = $3;
                           $$->attr.op = EQ;

			}
                        | additive-expression NEQ additive-expression
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

factor			: LPAREN expression RPAREN { $$ = $2; }
			| var { $$ = $1; }
			| call { $$ = $1; }
			| NUM	
			{
			   $$ = newExpNode(ConstK);
			   $$ = attr.val = atoi(tokenString);
			}
			;

call			: ID LPAREN args RPAREN
			{
			   $$ = newExpNode(CallK);
			   $$->child[0] = $3;
			   $$->attr.name = copyString(tokenString);
			}
			;

args			: arg-list 
			{
			   $$ = $1;
			}
			| empty	
			{
			   $$ = NULL;
			}
			;

arg-list		: arg-list COMMA expression
			{

			}
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

