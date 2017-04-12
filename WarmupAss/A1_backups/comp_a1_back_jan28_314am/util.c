#include "globals.h"
#include "util.c"


void printToken(int token, const char* tokenString){
	switch(token){
		case ENDFILE:
			break;
		case WORD:
			fprintf(listing,"WORD(%s)\n",tokenString);
			break;
		case NUMBER:
			fprintf(listing,"NUMBER(%s)\n",tokenString);
			break;
		case OPENTAG:
			fprintf(listing,"OPENTAG\n");
			break;
		case CLOSETAG:
			fprintf(listing,"CLOSETAG\n");
			break;
		default:
			fprintf(listing,"This should never happen!\n");
	}	
}
