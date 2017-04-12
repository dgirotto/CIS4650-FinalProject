#ifndef _GLOBALS_H_
#define _GLOBALS_H_


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef REL_TAG_CTR
#define REL_TAG_CTR 7
#endif

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
	ERROR,

	LSQBRK,
	RSQBRK,
	FSLASH,
	EQ,
	QUOTE

}TokenType;

#endif
