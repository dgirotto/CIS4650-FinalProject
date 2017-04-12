/*
** CIS*4650 A1 - util.c
** Desc: Contains the program's utility functions to help sanitize 
** and parse scanned input/tokens.
** Name: Daniel Girotto
** ID: 0783831
** Date: February 4, 2016
*/

#include "globals.h"
#include "util.h"

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
			return 1; //tag is relevant; return 1
		}
	}	
	return 0; //tag is not relevant; return 0
}

char* stripTag(char* toStrip,int type){
	int i;
	char* newTag;
	if(type==0){
		/* strip the open tag of its brackets */
		newTag = malloc(sizeof(char)*(strlen(toStrip)-2));
		for(i=0;i<(strlen(toStrip)-2);i++){
			newTag[i] = toStrip[i+1];
		}
	}	
	else if(type==1){
		/* strip the close tag of / and its brackets */
                newTag = malloc(sizeof(char)*(strlen(toStrip)-2));
                for(i=0;i<(strlen(toStrip)-3);i++){
                        newTag[i] = toStrip[i+2];
                }
	}
	else{ /* invalid type entered; don't return anything noteworthy */
		return NULL;
	}
	return newTag;
}
