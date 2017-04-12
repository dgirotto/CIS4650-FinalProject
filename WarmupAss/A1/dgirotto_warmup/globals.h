/*
** CIS*4650 A1 - globals.h
** Desc: Header file which defines the external variables along with
** the names of each of the tokens.
** Name: Daniel Girotto
** ID: 0783831
** Date: February 4, 2016
*/

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
extern char* relevantTags[];

/* the enumerator which defines each of the possible token types */
typedef enum{ 
	OPENTAG=1, 
	CLOSETAG,
	WORD,
	NUMBER, 
	APOST, 
	HYPHEN, 
	PUNCT,
	LSQBRK,
	RSQBRK,
	ENDFILE
}TokenType;

#endif
