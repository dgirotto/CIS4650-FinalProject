/* 
   File: globals.h
   Global types and vars for C MINUS compiler
   must come before other include files
*/


#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* for hashing function */
#define MAXSIZE 15
#define SHIFT 4


#define MAXCHILDREN 3

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */
extern int lineno; /* source line number for listing */

typedef enum{
	StmtK,ExpK,DecK
}NodeKind;

typedef enum{
	IfK,WhileK,ReturnK,CmpdK,AssignK
}StmtKind;

typedef enum{
	OpK,ConstK,IdK,CallK,SubsK
	//SubsK = a[3+i]
}ExpKind;

typedef enum{
	VarK,FunK,ParamK
}DecKind;

typedef struct treeNode{
	struct treeNode * child[MAXCHILDREN];
	struct treeNode * sibling;
	int lineno;

	/* for codegen */
	char * strval; // used for identifiers and numbers 
	int val; // used for ConstKind

	/* for typechecking */
	int type; //0 = void, 1 = int, 2 = void [], 3 = int [] ; used for type-checking
	int size; //the node's size if it happens to be an array

	NodeKind nodekind;
	union{
		StmtKind stmt;
		ExpKind exp;
		DecKind dec;
	}kind;
	union{
		int op;
		int val;
		char * name; 
	}attr;
}TreeNode;


typedef struct entry{
        char * name;
        int depth;
        TreeNode * theTree;
        struct entry * next;
}Entry;



/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

#endif

