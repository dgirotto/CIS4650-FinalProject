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
					printf("OPEN-%s\n",normTag);
                        	}
                       	 	else{
                                	relevant=0;
					newNode = createNode(normTag);
					push(badStack,newNode);
					//printStack(badStack);
                        	}
				captureTag=0;

			}		



			else if(ttype==RSQBRK && openTag==1){
				openTag=0;
			}
			else if(ttype!=CLOSETAG && captureTag==0 && openTag==0){ /* handles tokens between relevant tags*/
				if(ttype==WORD){
					printf("WORD(%s)\n",tokenString);
				}
				else if(ttype==HYPHEN){
					printf("HYPHENATED(%s)\n",tokenString);
				}
				else if(ttype==NUMBER){
					printf("NUMBER(%s)\n",tokenString);
				}	
				else if(ttype==APOST){
					printf("APOSTROPHIZED(%s)\n",tokenString);
				}
				else if(ttype==PUNCT){
					printf("PUNCTUATION(%s)\n",tokenString);
				}
			}


/* HAVE TO ACCOUNT FOR THIS CASE.... ONCE RELEVANT = 0, CANNOT ENTER THIS LOOP */
			else if(ttype==WORD && captureTag==1 && openTag==1){
				normTag = normalize(tokenString);
				if(checkRelevant(normTag)){
					/* if checkRelevant() returns 1 (true), then push the tag to the stack */
					newNode = createNode(normTag);
					push(mainStack,newNode);	
					//printStack(mainStack);
					printf("OPEN-%s\n",normTag);
				}
				else{
					relevant=0;
                                        newNode = createNode(normTag);
                                        push(badStack,newNode);

				}
				captureTag=0;	
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


        if(stackSize(mainStack) > 0){
                printf("\nThere are unmatched open tags in the stack! Unmatched tags:\n");
                printStack(mainStack);
        }

return 0;

}
