/*
** CIS*4650 A1 - stack.h
** Desc: Header file which contains the function definitions 
** for each of stack.c's functions. 
** Name: Daniel Girotto
** ID: 0783831
** Date: February 4, 2016
*/


#ifndef _STACK_H_
#define _STACK_H_

#include "globals.h"

/* StackNode - structure in which houses the name of a tag; designed 
to be a stack implementation */
struct StackNode{
	struct StackNode *next;
	char* tagName;	
};

typedef struct StackNode StackNode;

/* createStack - creates a StackNode pointer, allocates memory
towards it, returns its address
*/
StackNode* createStack();

/* createNode - accepts the tag name as a string (char pointer),
creates a StackNode, and stores the tag as StackNode's "tagName" member
*/
StackNode* createNode(char*);

/* push - gets passed a stack and the node to be pushed to the very
top of the stack; returns void
*/
void push(StackNode*,StackNode*);

/* pop - removes the very first (top) node of the stack and returns
it to the user in the form of a pointer 
*/
StackNode* pop(StackNode*);

/* top - returns the tag name of the node at the top of the stack 
(the stack's very first "*next" node) -- doesn't remove the node or tag, however
*/
char* top(StackNode*);

/* printStack - prints out each of the node's tag names in the stack 
from top to bottom
*/
void printStack(StackNode*);

/* stackSize - returns the number of elements (tag names) in the stack 
*/
int stackSize(StackNode*);

#endif
