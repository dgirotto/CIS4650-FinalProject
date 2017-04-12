#include "stack.h"

/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;
StackNode * mainStack; //creating the global stack


int main(int argc,char * argv[]){
	TokenType ttype;
	source = stdin;
	listing = stdout;

	mainStack = createStack();
	
	while((ttype = getToken()) != ENDFILE){
		printToken(ttype,tokenString);
	}

return 0;

}
