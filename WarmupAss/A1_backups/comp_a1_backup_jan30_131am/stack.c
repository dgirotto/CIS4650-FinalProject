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
	if(topNode != NULL){
		theStack->next = theStack->next->next;
		return topNode;
	}	
	printf("Cannot pop! Stack is empty.\n");
	return NULL;
}

int top(StackNode* theStack){
	if(theStack->next != NULL){
		TokenType topVal = theStack->next->theToken;
		return topVal;
	}
	return -1; //error; stack is empty  
}

void printStack(StackNode* theStack){
	StackNode* iter = theStack->next;
	printf("\n---TOP----\n");
	while(iter != NULL){
		printf("   %d\n",iter->theToken);
		iter = iter->next;
	}
	printf("--BOTTOM--\n\n");
}

