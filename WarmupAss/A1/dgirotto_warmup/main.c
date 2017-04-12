/*
** CIS*4650 A1 - main.c
** Desc: Main module which utilizes functions from util.c and stack.c.
** Name: Daniel Girotto
** ID: 0783831
** Date: February 4, 2016
*/

#include "util.h"
#include "globals.h"
#include "stack.h"
#include "scan.h"

char* relevantTags[] = {"TEXT","DATE","DOC","DOCNO","HEADLINE","LENGTH","P"};
/* if tag is not relevant (not part of the above list), disregard and treat as a comment */

FILE * source;
FILE * listing;

int main(int argc,char * argv[]){
	TokenType ttype;
	source = stdin;
	listing = stdout;
	
	StackNode* mainStack = createStack(); //used to store relevant tags
	StackNode* badStack = createStack(); //used to store irrelevant tags
	StackNode* newNode;
	
	char* normTag; //stores the normalized tagname (i.e. doc --> DOC)	
	int captureTag=0,openTag=0,relevant=1;

	while((ttype = getToken()) != ENDFILE){ 
	/* loop will continue to scan for input until the end of file token (ENDFILE) is found */
		if(relevant){ /* if the scanned token is relevant information ... */
			if(ttype==LSQBRK && captureTag==0 && openTag==0){ /* handles the "<" token */
				captureTag=1; //the next WORD needs to be recorded as as tag
				openTag=1; //the beginning of an "open tag"
			}
			else if(ttype==OPENTAG){ /* handles the <{word}> token */
				/* strip the token of < and > and convert the letters to uppercase */
				normTag = stripTag(tokenString,0); 
				normTag = normalize(normTag);

				/* the below code is redundant!!! */

				if(checkRelevant(normTag)){
					/* if checkRelevant() returns 1 (true), then push the tag to the stack */
					newNode = createNode(normTag);
					push(mainStack,newNode);
					fprintf(listing,"OPEN-%s\n",normTag);
				}
				else{
					relevant=0;
					newNode = createNode(normTag);
					push(badStack,newNode);
				}
				captureTag=0;

			}		
			else if(ttype==RSQBRK && openTag==1){ /* handles the ">" token (signifies the closing of an "open" tag) */
				openTag=0;
			}
			else if(ttype!=CLOSETAG && captureTag==0 && openTag==0){ /* handles tokens between relevant tags*/
				if(ttype==WORD){
					fprintf(listing,"WORD(%s)\n",tokenString);
				}
				else if(ttype==HYPHEN){
					fprintf(listing,"HYPHENATED(%s)\n",tokenString);
				}
				else if(ttype==NUMBER){
					fprintf(listing,"NUMBER(%s)\n",tokenString);
				}	
				else if(ttype==APOST){
					fprintf(listing,"APOSTROPHIZED(%s)\n",tokenString);
				}
				else if(ttype==PUNCT){
					fprintf(listing,"PUNCTUATION(%s)\n",tokenString);
				}
			}
			else if(ttype==WORD && captureTag==1 && openTag==1){
				normTag = normalize(tokenString);
				if(checkRelevant(normTag)){
					/* if checkRelevant() returns 1 (true), then push the tag to the stack */
					newNode = createNode(normTag);
					push(mainStack,newNode);
					fprintf(listing,"OPEN-%s\n",normTag);	
				}
				else{
					relevant=0;
					newNode = createNode(normTag);
					push(badStack,newNode);
				}	
				captureTag=0;
			}	
			else if(ttype==WORD && captureTag==0 && openTag==1){ // handles <p text-align="left">
				//an irrelevant attribute name or value within an open tag
			}
			else if(ttype==CLOSETAG){
				//check top of stack against the normalized close tag
				normTag = stripTag(tokenString,1);
				normTag = normalize(normTag);
				if(checkRelevant(normTag)){
					if(strcmp(normTag,top(mainStack))==0){
						pop(mainStack);
						fprintf(listing,"CLOSE-%s\n",normTag);
						relevant=1;
					}
					else{
						fprintf(listing,"ERROR! \"%s\" does not match the top of the stack!\n",normTag);
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
			}
			else if(ttype==CLOSETAG){ //i.e. </byline>
				normTag = stripTag(tokenString,1);
				normTag = normalize(normTag);
				if(strcmp(normTag,top(badStack))==0){ /* does the tag at the top of the stack match the scanned tag? */
					pop(badStack); /* the tags matched; pop the top tag off the stack */
					if(stackSize(badStack)==0){ /* if there are no more irrelevant tags left on the stack, input from this point on will be valid */
						relevant=1;
					}
				}
				else{
					fprintf(listing,"ERROR! \"%s\" does not match \"%s\"! Ignoring \"%s\".\n",normTag,top(badStack),normTag);
				}
			}
			/* the following three statements are needed to handle the "<byline align="center">" case */
			else if(ttype==LSQBRK && captureTag==0){ 
				openTag=1; /* indicates that we are within an open tag */
				captureTag=1; /* we will want to store the next WORD token as a tag name */
			}
			else if(ttype==WORD && captureTag==1){
				captureTag=0; /* we've capured the tag that we needed */
                                normTag = normalize(normTag);
                                newNode = createNode(normTag);
                                push(badStack,newNode);
			}
			else if(ttype==RSQBRK && openTag==1){
				openTag=0; /* indicates we are no longer within an open tag */
			}
			
			else{
				//printf("WITHIN IRRELEVANT TAGS & NOT CAPTURED BY ANY OF THE IF STATEMENTS: %s\n",tokenString);
			}
		}
	}

	/* determine if there are any unmatched open tags in the stack, and display them to the console */
        if(stackSize(mainStack) > 0 || stackSize(badStack) > 0){
		fprintf(listing,"\nThere are unmatched open tags in the stack! They are:\n");        
                printStack(mainStack);
		printStack(badStack);
        }

return 0;

}

