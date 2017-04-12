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

%nonassoc LT LTEQ GT GTEQ NEQ
%left PLUS MINUS
%left TIMES OVER

%% 

/* GRAMMAR FOR CMINUS */

program			: declaration_list
			{
			   savedTree = $1; /* could also do: $$ = $1, then savedTree = $$ */
			}
			;

declaration_list	: declaration_list declaration 
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
			| declaration { $$ = $1; }
			;

declaration		: var_declaration { $$ = $1; }
			| fun_declaration { $$ = $1; }
			;

var_declaration		: type_specifier ID SEMI
			{
			   $$ = newDecNode(VarK);
			   $$->child[0] = $1;
			   $$->attr.name = copyString(tokenString); //store the ID

			}
			| type_specifier ID LSQ NUM RSQ SEMI
			{
			   $$ = newDecNode(VarK);
			   $$->child[0] = $1;
			   $$->attr.name = copyString(tokenString);
			   // NEED TO STORE BOTH ID AND NUM ... ???
 
			}
			;

type_specifier		: INT 
			{ 
			   // need to store "INT"???
			}
			| VOID 
			{ 
			   // need to store "VOID" ???
			}
			;

fun_declaration 	: type_specifier ID LPAREN params RPAREN compound_stmt
			{
			   $$ = newDecNode(FunK);
			   $$->child[0] = $1;
			   $$->child[1] = $4;
			   $$->child[2] = $6;
			   $$->attr.name = copyString(tokenString);
			}
			;

params			: param_list
			{
			   $$ = $1;
			}
			| VOID
			{
			   // NEED TO STORE "VOID" ??
			   $$ = $1;
			}
			;

param_list		: param_list COMMA param
			{
			   TreeNode * tree = $1;
                           if(tree != NULL){ /* if the tree is not null ... */
                              while(tree->sibling != NULL){ /* find an empty (NULL) sibling  */
                                 tree = tree->sibling;
                              }
                              tree->sibling = $3; 
                              $$ = $1; 
                           }
                           else{  // the tree is currently empty
                              $$ = $3; /* store the tree's first node */
                           }

			}
			| param
			{
			   $$ = $1;
			}
			;

param			: type_specifier ID 
			{
			   $$ = newDecNode(ParamK);
			   $$->child[0] = $1;
			   $$->attr.name = copyString(tokenString);
			}
			| type_specifier ID LSQ RSQ
			{
			   $$ = newDecNode(ParamK);
			   $$->child[0] = type-specifier;
			   $$->attr.name = copyString(tokenString);
			}
			;

compound_stmt		: LCURLY local_declarations statement_list RCURLY
			{
			   $$ = newStmtNode(CmpdK);
			   $$->child[0] = $2;
			   $$->child[1] = $3;
			   
			}
			;

local_declarations	: /* empty */ 
			| local_declarations var_declaration 
			{
			   TreeNode * tree = $1;
                           if(tree != NULL){ /* if the tree is not null ... */
                              while(tree->sibling != NULL){ /* find an empty (NULL) sibling  */
                                 tree = tree->sibling;
                              }
                              tree->sibling = $2;
                              $$ = $1;
                           }
                           else{  // the tree is currently empty
                              $$ = $2; /* store the tree's first node */
                           }
			}
			;

statement_list		: statement_list statement
			{
			   TreeNode * tree = $1;
                           if(tree != NULL){ /* if the tree is not null ... */
                              while(tree->sibling != NULL){ /* find an empty (NULL) sibling  */
                                 tree = tree->sibling;
                              }
                              tree->sibling = $2;
                              $$ = $1;
                           }
                           else{  // the tree is currently empty
                              $$ = $2; /* store the tree's first node */
                           }
			}
			;

statement		: expression_stmt { $$ = $1; }
			| compound_stmt { $$ = $1; }
			| selection_stmt { $$ = $1; }
			| iteration_stmt { $$ = $1; }
			| return_stmt { $$ = $1; }
			;

expression_stmt		: expression SEMI
			{
			   $$ = $1; // is this right ??
			}
			| SEMI 
			{ 
			   // not sure what to do here
			}
			;

selection_stmt		: IF LPAREN expression RPAREN statement
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

iteration_stmt		: WHILE LPAREN expression RPAREN statement
			{
			   $$ = newStmtNode(WhileK);
			   $$->child[0] = $3;
			   $$->child[1] = $5;
			}
			;

return_stmt 		: RETURN SEMI
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
			| simple_expression { $$ = $1; }
			;

var			: ID
			{
			   // what is savedName and savedLineNo ???
			   $$ = newExpNode(IdK);
			   $$->attr.name = copyString(tokenString); // implement copyString() ... what/where is "tokenString" ???	
			}
			| ID LSQ expression RSQ
			{
			   // not sure if this is correct  (probably not correct ) ...
			   $$ = newExpNode(IdK);
			   $$->attr.name = copyString(tokenString);
			   $$->child[0] = $3;
			   
			}
			;

simple_expression	: additive_expression LT additive_expression
			{
			   $$ = newExpNode(OpK);
			   $$->child[0] = $1;
			   $$->child[1] = $3;
			   $$->attr.op = LT; 
			}
			| additive_expression LTEQ additive_expression
			{
                           $$ = newExpNode(OpK);
                           $$->child[0] = $1;
                           $$->child[1] = $3;
                           $$->attr.op = LTEQ;

			}
                        | additive_expression GT additive_expression
                        {
                           $$ = newExpNode(OpK);
                           $$->child[0] = $1;
                           $$->child[1] = $3;
                           $$->attr.op = GT;

			}
                        | additive_expression GTEQ additive_expression
                        {
                           $$ = newExpNode(OpK);
                           $$->child[0] = $1;
                           $$->child[1] = $3;
                           $$->attr.op = GTEQ;
		
			}
                        | additive_expression EQ additive_expression
                        {
                           $$ = newExpNode(OpK);
                           $$->child[0] = $1;
                           $$->child[1] = $3;
                           $$->attr.op = EQ;

			}
                        | additive_expression NEQ additive_expression
                        {
                           $$ = newExpNode(OpK);
                           $$->child[0] = $1;
                           $$->child[1] = $3;
                           $$->attr.op = NEQ;
	
			}
			| additive_expression 
			{
                           // what to do here?
			   $$ = $1; // ??
			}
			;


additive_expression	: additive_expression PLUS term
			{
			   $$ = newExpNOde(OpK);
			   $$->child[0] = $1;
			   $$->child[1] = $3;
			   $$->attr.op = PLUS;
			}
			| additive_expression MINUS term
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

args			: 
			| arg_list 
			{
			   $$ = $1;
			}
			;

arg_list		: arg_list COMMA expression
			{
			   TreeNode * tree = $1;
                           if(tree != NULL){ /* if the tree is not null ... */
                              while(tree->sibling != NULL){ /* find an empty (NULL) sibling  */
                                 tree = tree->sibling;
                              }
                              tree->sibling = $3;
                              $$ = $1;
                           }
                           else{  // the tree is currently empty
                              $$ = $3; /* store the tree's first node */
                           }

			}
			| expression 
			{
			   $$ = $1; // is this right?
			}
			;
