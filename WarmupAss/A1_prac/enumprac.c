#include <stdio.h>

typedef enum{
	WHITE=10,
	BLUE=20,
	GREEN=30
}LeafClrs;

void printEnum(LeafClrs enumVal){
	printf("%d\n",enumVal);
}


int main(){


	printEnum(GREEN);
	/*LeafClrs a = GREEN;
	if(a==WHITE){
		printf("a is WHITE\n");
	}
	else{
		printf("a is %d\n",a);
	}*/
return 0;
}
