#include "util.h"
#include "globals.h"
#include "stack.h"
#include "scan.h"

/* allocate global variables */


char* relevantTags[] = {"TEXT","DATE","DOC","DOCNO","HEADLINE","LENGTH","P"};
/* if tag is not relevant (not part of the above list), disregard and treat as a comment */

int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;
StackNode * mainStack; //creating the global stack

/* checkRelevant() checks if the passed string (tag) matches any
of the "relevant" tags described above. Returns 1 if the passed 
tag is relevant (match is found); 0 otherwise */

char* normalize(char* theTag){
	char* newTag = malloc(sizeof(char)*strlen(theTag));
	int i;
	for(i=0; i<strlen(theTag); i++){
		newTag[i] = toupper(theTag[i]);
	}	
	printf("Inside normalize()... returning: %s\n",newTag);
	return newTag;
}

int checkRelevant(char* theTag){
	int i;
	for(i=0; i<REL_TAG_CTR; i++){
		if(strcmp(theTag,relevantTags[i]) == 0){
			printf("%s is relevant!\n",theTag);
			return 1; //tag is relevant; return 1
		}
	}	
	printf("%s is NOT relevant!\n",theTag);
	return 0; //tag is not relevant; return 0
}


int main(int argc,char * argv[]){
	TokenType ttype;
	source = stdin;
	listing = stdout;
	char* normTag; //stores the normalized tagname (i.e. doc --> DOC)	
	StackNode* newNode;

	mainStack = createStack();

	while((ttype = getToken()) != ENDFILE){
		if(ttype==WORD){
			printf("The token type is WORD\n");
			normTag = normalize(tokenString);
			if(checkRelevant(normTag)){
				/* if checkRelevant() returns 1 (true), then push the tag to the stack */
				newNode = createNode(ttype);
				push(mainStack,newNode);	
				printStack(mainStack);
			}
		}
		printToken(ttype,tokenString);
	}

return 0;

}
