#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern int lineno; /* source line number for listing */

typedef enum{ 
	// UNKNOWN = 0
	OPEN_TAG=1, 
	CLOSE_TAG, //=2 
	WORD,  //=3
	NUMBER, 
	APOST, 
	HYPHEN, 
	PUNCT,
	ENDFILE
}TokenType;
