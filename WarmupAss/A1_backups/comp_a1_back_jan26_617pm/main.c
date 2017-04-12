#include "stack.h"

int main(){
        StackNode* myStack = createStack();

        StackNode* newNode = createNode(WORD);
        push(myStack,newNode);

        newNode = createNode(NUMBER);
        push(myStack,newNode);

        newNode = createNode(PUNCTUATION);
        push(myStack,newNode);

        printStack(myStack);


return 0;

}
