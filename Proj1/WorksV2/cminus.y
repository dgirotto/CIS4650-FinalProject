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
			| type_specifier ID
			{
			   savedName = copyString(tokenString); //store the ID
                           savedLineNo = lineno;
			}
			LSQ NUM 
			{
			   // record the NUM
			   printf("Recording NUM : %s\n",tokenString);
			   savedNum = atoi(tokenString);
			   /*
			   $$ = newExpNode(ConstK);
			   $$ = attr.val = atoi(tokenString); 
			   */
			}
			RSQ SEMI
			{
			   $$ = newDecNode(VarK);
                           $$->child[0] = $1;



			   /* child[1] will hold a "const" -- the size of the array */
			   $$->child[1] = newExpNode(ConstK);
			   $$->child[1]->attr.val = savedNum;
			   /* NOT SURE IF ABOVE IS CORRECT */ 	

 
                           $$->attr.name = savedName; //store the ID
			}
			;

type_specifier		: INT
			{
			   //create const node, assign it 1
			   $$ = newExpNode(ConstK);
			   $$->attr.val = 1;
			}
			| VOID 
			{
			   //create const node, assign it 0
			   $$ = newExpNode(ConstK);
                           $$->attr.val = 0;
			}
			;

fun_declaration 	: type_specifier ID 
			{
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

params			: param_list { $$ = $1; }
			| VOID
			{
                           //create const node, assign it 0
                           $$ = newExpNode(ConstK);
                           $$->attr.val = 0;
                        }
			;

param_list		: param_list COMMA param
			{
			   TreeNode * tree = $1;
			   if(tree != NULL){
			      while(tree->sibling != NULL){ 
                                 tree = tree->sibling;
                              }
                              tree->sibling = $3; 
                              $$ = $1; 
                           }
                           else{ 
                              $$ = $3;
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
			| type_specifier ID
			{
			   savedName = copyString(tokenString);
			   savedLineNo = lineno;
			}
			LSQ RSQ
			{
			   printf("Matched an 'int ID[]'\n");
			   $$ = newDecNode(ParamK);
                           $$->child[0] = $1;
                           $$->attr.name = savedName;
			   $$->lineno = savedLineNo;
			}
			;

compound_stmt		: LCURLY local_declarations statement_list RCURLY
			{
			   $$ = newStmtNode(CmpdK);
			   $$->child[0] = $2;
			   $$->child[1] = $3;   
			}
			;

local_declarations	: { $$ = NULL; } /* empty */ 
			| local_declarations var_declaration
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
			;

statement_list		: { $$ = NULL; } /* empty */
			| statement_list statement
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
			;

statement		: expression_stmt { $$ = $1; }
			| compound_stmt { $$ = $1; }
			| selection_stmt { $$ = $1; }
			| iteration_stmt { $$ = $1; }
			| return_stmt { $$ = $1; }
			;

expression_stmt		: expression SEMI { $$ = $1; }
			| SEMI 
			{
			   $$ = NULL; //??
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
			   $$ = newStmtNode(ReturnK);  // ???
			}
			| RETURN expression SEMI
			{
			   $$ = newStmtNode(ReturnK);
			   $$->child[0] = $2;
			}
			;

expression		: var ASSIGN expression
			{
			   /* IS THIS CORRECT? */
			   $$ = newStmtNode(AssignK);
			   $$->child[0] = $1;
			   $$->child[1] = $3;
			} 
			| simple_expression { $$ = $1; }
			;

var			: ID
			{
			   $$ = newExpNode(IdK);
			   $$->attr.name = copyString(tokenString);
			}
			| ID 
			{
			   savedName = copyString(tokenString); //store the ID
                           savedLineNo = lineno;  
			}
			LSQ expression RSQ
			{
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
			   $$ = $1;
			}
			;

additive_expression	: additive_expression PLUS term
			{
			   $$ = newExpNode(OpK);
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
			| term { $$ = $1; }
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
			| factor { $$ = $1; }
			;

factor			: LPAREN expression RPAREN
			{
			   $$ = $2; // ??
			}
			| var { $$ = $1; }
			| call { $$ = $1; }
			| NUM
			{
			   $$ = newExpNode(ConstK);
			   $$->attr.val = atoi(tokenString);
			}	
			;

call			: ID
			{
			   savedName = copyString(tokenString);	
			   savedLineNo = lineno;
			} 
			LPAREN args RPAREN
			{
			   $$ = newExpNode(CallK);
			   $$->child[0] = $3;
			   $$->attr.name = savedName;
			   $$->lineno = savedLineNo;
			}
			;

args			: { $$ = NULL; } /* empty */
			| arg_list { $$ = $1; }
			;

arg_list		: arg_list COMMA expression
			{
			   TreeNode * tree = $1;
                           if(tree != NULL){ 
                              while(tree->sibling != NULL){
                                 tree = tree->sibling;
                              }
                              tree->sibling = $3;
                              $$ = $1;
                           }
                           else{
                              $$ = $3;
                           }	
			}
			| expression { $$ = $1; }
			;
