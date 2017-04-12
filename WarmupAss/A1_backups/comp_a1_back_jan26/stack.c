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
	TokenType topToken;	
	return topToken; 
}

int isEmpty(StackNode* theStack){
	if(theStack->next == NULL){
		return 1; //stack is empty, return 1
	}
	return 0;
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

int main(){
	StackNode* myStack = createStack();

	StackNode* newNode = createNode(WORD);
        push(myStack,newNode);

     	newNode = createNode(NUMBER);
        push(myStack,newNode);

        newNode = createNode(PUNCTUATION);
 	push(myStack,newNode);	

	printStack(myStack);	
	pop(myStack);
	pop(myStack);
	printStack(myStack);
	pop(myStack);
	printStack(myStack);
	pop(myStack);
	pop(myStack);

	newNode = createNode(OPENTAG);
	push(myStack,newNode);
	printStack(myStack);
	newNode = createNode(CLOSETAG);
	push(myStack,newNode);
	printStack(myStack);
return 0;
}
