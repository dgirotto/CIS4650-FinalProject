/*
CIS*4650 A1 - util.c
Name: Daniel Girotto
Date: February 4, 2016
ID: 0783831
*/


#include "globals.h"
#include "util.h"

/* normalize() makes each of the passed tag's characters upper-case.
(i.e. Hello becomes HELLo) 
*/
char* normalize(char* theTag){
	char* newTag = malloc(sizeof(char)*strlen(theTag));
	int i;
	for(i=0; i<strlen(theTag); i++){
		newTag[i] = toupper(theTag[i]);
	}	
	return newTag;
}

/* checkRelevant() checks if the passed string (tag) matches any
of the "relevant" tags described above. Returns 1 if the passed 
tag is relevant (match is found); 0 otherwise 
*/
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

/* stripTag() receives either an open or close tag in its original form
(<TAG> or </TAG>) and strips the string of its brackets (i.e. <TAG> becomes TAG)
PARAMS:
	toStrip: pointer to the tag to be "stripped"
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
