#include "stack.h"

int main(){

StackNode* theStack;
theStack = createStack();
StackNode* newNode;


char test[] = "hello";
char* abc;
abc = malloc(sizeof(char)*strlen(test));
strcpy(abc,test);
printf("%s\n",abc);

newNode = createNode("DOC");
theStack->next = newNode;
printf("%s\n",theStack->next->tagName);
//push(theStack,newNode);

//printf("%s\n",theStack->next->tagName);

//newNode = createNode("LENGTH");
//push(theStack,newNode);

//printf("%s\n",theStack->tagName);
//printf("%s\n",theStack->next->next->tagName);

//newNode = createNode("P");
//push(theStack,newNode);

//printStack(theStack);


return 0;
}
