#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct StackNode{
	struct StackNode *next;
	char* tagName;	
};

typedef struct StackNode StackNode;

StackNode* createStack();
StackNode* createNode(char*);
void push(StackNode*,StackNode*);
StackNode* pop(StackNode*);
char* top(StackNode*);
void printStack(StackNode*);

#endif
