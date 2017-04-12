/****************************************************/
/* File: util.h                                     */
/* Utility functions for the TINY compiler          */
/****************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_


void visit(TreeNode*);
void postorder(TreeNode*);

/* symbol table functions */
Entry* lookup(char*,int,int);

void deleteScope(int);

int delete(char*);

int insert(char*,TreeNode*,int);

/* function which computes a hash value */
int hash(char*);

/* prints each of the error messages out to the user */
void printErrors(void); 

/* records the error message along with the line the error is on */
void addErrMsg(char*,int);

/* prints a token and its value to the listing file
 */
void printToken( int, const char* );

/* creates a new declaration node for syntax tree construction */
TreeNode * newDecNode(DecKind);

/* creates a new statement node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind);

/* creates a new expression node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind);

/* allocates and makes a new copy of an existing string
 */
char * copyString( char * );

/* prints a syntax tree to the * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * );

#endif

