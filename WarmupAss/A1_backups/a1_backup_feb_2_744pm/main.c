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
	return newTag;
}

int checkRelevant(char* theTag){
	int i;
	for(i=0; i<REL_TAG_CTR; i++){
		if(strcmp(theTag,relevantTags[i]) == 0){
			//printf("%s is relevant!\n",theTag);
			return 1; //tag is relevant; return 1
		}
	}	
	return 0; //tag is not relevant; return 0
}

/* removes "<",">", and "/"  from tags (i.e. <hello> --> hello)  
type=0: strip open tag of < and >
type=1: strip close tag of </ and >
*/
char* stripTag(char* toStrip,int type){
	int i;
	char* newTag;
	if(type==0){
		newTag = malloc(sizeof(char)*(strlen(toStrip)-2));
		for(i=0;i<(strlen(toStrip)-2);i++){
			newTag[i] = toStrip[i+1];
		}
	}	
	else if(type==1){
                newTag = malloc(sizeof(char)*(strlen(toStrip)-2));
                for(i=0;i<(strlen(toStrip)-3);i++){
                        newTag[i] = toStrip[i+2];
                }
	}
	else{
		printf("Invalid type!\n");
		return NULL;
	}
	return newTag;
}

int main(int argc,char * argv[]){
	TokenType ttype;
	source = stdin;
	listing = stdout;
	char* normTag; //stores the normalized tagname (i.e. doc --> DOC)	
	StackNode* newNode;

	int captureTag=0,openTag=0,relevant=1;

	mainStack = createStack();
	StackNode* badStack = createStack(); //used to store irrelevant tags

	while((ttype = getToken()) != ENDFILE){
		if(relevant){
			if(ttype==LSQBRK && captureTag==0 && openTag==0){
				captureTag=1; //the next WORD needs to be recorded as as tag
				openTag=1; //the beginning of an "open tag"
			}
		

			else if(ttype==OPENTAG){
				normTag = stripTag(tokenString,0);
				normTag = normalize(normTag);

				/* the below code is redundant!!! */

				if(checkRelevant(normTag)){
					//relevant=1;
                                	/* if checkRelevant() returns 1 (true), then push the tag to the stack */
                               		newNode = createNode(normTag);
                                	push(mainStack,newNode);
                                	//printStack(mainStack);
                                	captureTag=0;
					printf("OPEN-%s\n",normTag);
                        	}
                       	 	else{
                                	relevant=0;
					newNode = createNode(normTag);
					push(badStack,newNode);
					//printStack(badStack);
                        	}

			}		



			else if(ttype==RSQBRK && openTag==1){
				openTag=0;
			}
			else if(ttype!=CLOSETAG && captureTag==0 && openTag==0){ /* handles tokens between relevant tags*/
				if(ttype==WORD){
					printf("WORD(%s)\n",tokenString);
				}
				else if(ttype==HYPHEN){
					printf("HYPHEN(%s)\n",tokenString);
				}
				else if(ttype==NUMBER){
					printf("NUMBER(%s)\n",tokenString);
				}	
				else if(ttype==APOST){
					printf("APOSTROPHE(%s)\n",tokenString);
				}
			}
			else if(ttype==WORD && captureTag==1 && openTag==1){
				normTag = normalize(tokenString);
				if(checkRelevant(normTag)){
					/* if checkRelevant() returns 1 (true), then push the tag to the stack */
					newNode = createNode(normTag);
					push(mainStack,newNode);	
					//printStack(mainStack);
					captureTag=0;
					printf("OPEN-%s\n",normTag);
				}
				else{
					relevant=0;
				}	
			}	
			else if(ttype==WORD && captureTag==0 && openTag==1){ // handles <p text-align="left">
				//printf("Ignoring %s\n",tokenString);
			}
			else if(ttype==CLOSETAG){
				//check top of stack against the normalized close tag
				normTag = stripTag(tokenString,1);
				normTag = normalize(normTag);
				if(checkRelevant(normTag)){
					if(strcmp(normTag,top(mainStack))==0){
						pop(mainStack);
						//printStack(mainStack);
						printf("CLOSE-%s\n",normTag);
						relevant=1;
					}
					else{
						printf("ERROR!! %s DOES NOT MATCH THE TOP OF THE STACK!\n",normTag);
					}
				}
			}
			else{
				//printf("ELSE case\n");
			}	
		}//end of first if
		else{ //relevant==0
			//test if open tag
			if(ttype==OPENTAG){ //i.e. <byline>
				normTag = stripTag(tokenString,0);
				normTag = normalize(normTag);
				newNode = createNode(normTag);
				push(badStack,newNode);
				//printStack(badStack);
			}
			else if(ttype==CLOSETAG){ //i.e. </byline>
				normTag = stripTag(tokenString,1);
				normTag = normalize(normTag);
				if(strcmp(normTag,top(badStack))==0){ /* does the tag at the top of the stack match the scanned tag? */
					pop(badStack); /* the tags matched; pop the top tag off the stack */
					//printStack(badStack);
					if(stackSize(badStack)==0){ /* if there are no more irrelevant tags left on the stack, input from this point on will be valid */
						relevant=1;
					}
				}
				else{
					printf("%s DOES NOT MATCH %s\n",normTag,top(badStack));
				}
			}
			/* the following three statements are needed to handle the "<byline align="center">" case */
			else if(ttype==LSQBRK && captureTag==0){ 
				openTag=1; /* indicates that we are within an open tag */
				captureTag=1; /* we will want to store the next WORD token as a tag name */
				//printf("FOUND A '<' WITHIN IRRELEVANT TAGS\n");
			}
			else if(ttype==WORD && captureTag==1){
				captureTag=0; /* we've capured the tag that we needed */
				//printf("FOUND A 'WORD' WITHIN IRRELEVANT TAGS:(%s)\n",tokenString);
                                normTag = normalize(normTag);
                                newNode = createNode(normTag);
                                push(badStack,newNode);
                                //printStack(badStack);

			}
			else if(ttype==RSQBRK && openTag==1){
				openTag=0; /* indicates we are no longer within an open tag */
				//printf("FOUND A '>' WITHIN IRRELEVANT TAGS\n");
			}
			
			else{
				//printf("WITHIN IRRELEVANT TAGS & NOT CAPTURED BY ANY OF THE IF STATEMENTS: %s\n",tokenString);
			}
		}
		//printToken(ttype,tokenString);
	}

return 0;

}
