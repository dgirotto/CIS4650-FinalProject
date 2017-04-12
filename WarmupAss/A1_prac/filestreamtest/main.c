#include "test.h"
extern FILE * outputFile = NULL;


int main(){
	outputFile = stdout;
	fprintf(outputFile,"hello\n");
	test();
return 0;
}
