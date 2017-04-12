/*
** CIS*4650 A1 - stack.c
** Desc: File in which facilitates the functionalities
** of a comprehensive FIFO stack data structure.
** Name: Daniel Girotto
** ID: 0783831
** Date: February 4, 2016
*/

#include "stack.h"

StackNode* createStack(){
	StackNode* newStack = malloc(sizeof(StackNode));
	/* set the node's initial values to NULL as to ensure they behave 
	properly when manipulating them in the future */
	newStack->next = NULL;
	newStack->tagName = NULL;
	return newStack;
}

StackNode* createNode(char* tagName){
	StackNode *newNode = malloc(sizeof(StackNode));
	newNode->next = NULL;
	newNode->tagName = malloc(sizeof(char)*strlen(tagName));
	/* store the name of the tag inside the node's "tagName" element */
	strcpy(newNode->tagName,tagName);
	return newNode;
}

void push(StackNode* theStack,StackNode* toPush){
	if(theStack->next != NULL){
		/* if the stack already contains elements, shift the existing first element
		to appear after the new element "toPush" */
		toPush->next = theStack->next;
	}
	theStack->next = toPush;
}

StackNode* pop(StackNode* theStack){
	StackNode* topNode = theStack->next;
	if(topNode != NULL){
		/* above if statment is meant for error checking: if the stack actually
		contains element(s) ... */
		theStack->next = theStack->next->next;
		return topNode; /* exit the function; do not proceed past the if statement block */
	}	
	printf("Cannot pop! Stack is empty.\n");
	return NULL;
}

char* top(StackNode* theStack){
	if(theStack->next != NULL){
		return theStack->next->tagName;
	}
	return NULL; //error; stack is empty  
}

void printStack(StackNode* theStack){
	StackNode* iter = theStack->next;
	while(iter != NULL){
		printf("%s\n",iter->tagName);
		iter = iter->next;
	}
}

int stackSize(StackNode* theStack){
	int size=0;
	StackNode* iter = theStack;
	/* iterate through each of the nodes in the stack, if a particular node's "next" element
	does not exist (is NULL), stop the iteration process */
	while(iter->next != NULL){
		size++;
		iter = iter->next;
	}
	return size;
}

