/* 
File: main.c
Main program for CMINUS compiler                   
*/

#include "globals.h"

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE

#include "util.h"
#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#endif


/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

/* allocate and set tracing flags */
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;

main( int argc, char * argv[] ){

	insert("daniel",NULL,1);
	insert("joseph",NULL,2);
	insert("girotto",NULL,2);
	insert("test",NULL,2);
	insert("test2",NULL,2);
	insert("dane",NULL,2);
	insert("drew",NULL,2);
	insert("drew",NULL,2);
	insert("jeff",NULL,2);
	insert("jon",NULL,2);
	insert("mike",NULL,2);
	display();
	printf("\n");

	delete("girotto");
	delete("mike");
	delete("test2");
	display();
	printf("\n");

	insert("girotto",NULL,1);
	insert("test2",NULL,1);
	display();

	/*
	Entry* lookup(char*);
	int delete(char*);
	int insert(char*,TreeNode*,int);
	*/

	TreeNode * syntaxTree;
	int ttype;
	char pgm[120]; /* source code file name */

	if(argc != 2){ 
		fprintf(stderr,"usage: %s <filename>\n",argv[0]);
		exit(1);
	}

	strcpy(pgm,argv[1]) ;
	if(strchr (pgm, '.') == NULL){
		strcat(pgm,".cm");
	}
	source = fopen(pgm,"r");

	if(source==NULL){
		fprintf(stderr,"File %s not found\n",pgm);
		exit(1);
	}
	listing = stdout; /* send listing to screen */
	fprintf(listing,"\nTINY COMPILATION: %s\n",pgm);

#if NO_PARSE
	while( (ttype=getToken())!= 0 ){
		printToken( ttype, tokenString );
	}

#else
	syntaxTree = parse();
	if (TraceParse) {
		fprintf(listing,"\nSyntax tree:\n");
		printTree(syntaxTree);
  	}
	fprintf(listing,"\n");
	printErrors();
#endif
	fclose(source);
	return 0;
}
