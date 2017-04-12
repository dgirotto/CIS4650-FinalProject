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

	int captureTag=0,openTag=0;

	mainStack = createStack();

	while((ttype = getToken()) != ENDFILE){

		if(ttype==LSQBRK && captureTag==0 && openTag==0){
			captureTag=1; //the next WORD needs to be recorded as as tag
			openTag=1; //the beginning of an "open tag"
			printf("captureTag=1 and openTag=1\n");
		}
		else if(ttype==RSQBRK && openTag==1){
			openTag=0;
			printf("openTag=0\n");
		}
		else if(ttype==WORD && captureTag==1 && openTag==1){
			printf("The token type is WORD and captureTag==1 and openTag==1\n");
			normTag = normalize(tokenString);
			if(checkRelevant(normTag)){
				/* if checkRelevant() returns 1 (true), then push the tag to the stack */
				newNode = createNode(normTag);
				push(mainStack,newNode);	
				printStack(mainStack);
				captureTag=0;
			}
			else{
				printf("%s is NOT relevant!\n",normTag);
			}
		}
		else if(ttype==WORD && captureTag==0 && openTag==1){
			printf("Ignoring %s\n",tokenString);
		}
		else if(ttype==CLOSETAG){
			//check top of stack against the normalized close tag
			printf("The close tag: %s\n",tokenString);
			printf("length of the string: %d\n",strlen(tokenString));
		}
		else{
			printf("ELSE case\n");
		}	
		//printToken(ttype,tokenString);
	}

return 0;

}
