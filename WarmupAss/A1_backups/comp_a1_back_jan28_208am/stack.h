#include "globals.h"

struct StackNode{
	struct StackNode *next;
	TokenType theToken;	
};

typedef struct StackNode StackNode;

StackNode* createStack();
StackNode* createNode(TokenType);
void push(StackNode*,StackNode*);
StackNode* pop(StackNode*);
int top(StackNode*);
void printStack(StackNode*);
