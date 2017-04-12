/****************************************************/
/* File: util.h                                     */
/* Utility functions for the TINY compiler          */
/****************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

/* displays the amount of collected symantic errors to the user (called by main) */
void semanticCheck(void);

/* iterates through the tree in a post order fashion and collects the program's semantic data */
void postorder(TreeNode*);

/* displays the contents of the hash table (used for testing/dev purposes) */
void display(void);

/* inserts an entry into the symbol table (returns zero if unsuccessful)  */
int insert(char*,TreeNode*,int);

/* returns the symbol table entry to the user */
Entry* lookup(char*,int,int);

/* removes every entry in the symbol table that exists at the given scope */
void deleteScope(int);

/* function which computes a hash value for hash table insertion */
int hash(char*);

/* prints each of the error messages out to the user */
void printErrors(void); 

/* records the error message along with the line the error is on */
void addErrMsg(char*,int);

/* prints a token and its value to the listing file */
void printToken( int, const char* );

/* creates a new declaration node for syntax tree construction */
TreeNode * newDecNode(DecKind);

/* creates a new statement node for syntax tree construction */
TreeNode * newStmtNode(StmtKind);

/* creates a new expression node for syntax tree construction */
TreeNode * newExpNode(ExpKind);

/* allocates and makes a new copy of an existing string */
char * copyString( char * );

/* prints a syntax tree to the * listing file using indentation to indicate subtrees */
void printTree( TreeNode * );

#endif

