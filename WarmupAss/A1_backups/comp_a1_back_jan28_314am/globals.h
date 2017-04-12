#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern int lineno; /* source line number for listing */

typedef enum{ 
	// UNKNOWN = 0
	OPENTAG=1, 
	CLOSETAG,
	WORD,
	NUMBER, 
	APOST, 
	HYPHEN, 
	PUNCT,
	ENDFILE,
	ERROR

}TokenType;
