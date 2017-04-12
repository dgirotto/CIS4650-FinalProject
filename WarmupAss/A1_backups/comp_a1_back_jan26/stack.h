#include "globals.h"

struct StackNode{
	struct StackNode *next;
	TokenType theToken;	
};

typedef struct StackNode StackNode;

void push(StackNode*,StackNode*);
StackNode* pop(StackNode*);
int top(StackNode*);
int isEmpty(StackNode*);
