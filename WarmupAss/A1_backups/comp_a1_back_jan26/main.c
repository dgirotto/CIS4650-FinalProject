#include "globals.h"

typedef enum{
	WHITE=10,
	BLUE=20,
	GREEN=30
}LeafClrs;

int main(){
	LeafClrs a = GREEN;
	if(a==WHITE){
		printf("a is WHITE\n");
	}
	else{
		printf("a is %d\n",a);
	}
return 0;
}
