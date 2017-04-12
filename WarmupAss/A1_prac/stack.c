#include "stack.h"

StackNode* createStack(){
	StackNode* newStack = malloc(sizeof(StackNode));
	return newStack;
}
StackNode* createNode(TokenType theToken){
	StackNode *newNode = malloc(sizeof(StackNode));
	newNode->next=NULL;
	newNode->theToken = theToken;
}

void push(StackNode* theStack,StackNode* toPush){
	if(theStack->next != NULL){
		toPush->next = theStack->next;
	}
	theStack->next = toPush;
}

StackNode* pop(StackNode* theStack){
	StackNode* topNode = theStack->next;
	theStack->next = theStack->next->next;
	return topNode;
}

int top(StackNode* theStack){
	TokenType topToken;	
	return topToken; 
}

int isEmpty(StackNode* theStack){
	if(theStack->next == NULL){
		return 1; //stack is empty, return 1
	}
	return 0;
}
int main(){
	TokenType tt = WORD;
	StackNode* myStack = createStack();
	StackNode* newNode = createNode(tt);
	
	
return 0;
}
