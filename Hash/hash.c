#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 19937
#define SHIFT 4

int hash(char * key){
	int temp=0;
	int i=0;
	while(key[i] != '\0'){
		temp = ((temp << SHIFT) + key[i]) % MAXSIZE;
		printf("temp = %d\n",temp);
		i++;
	}
	return temp;
}

int main(void){
	int x=0;
	char * theKey = malloc(10); // allocate 10 bytes 

	strcpy(theKey,"Hello");
	printf("%d\n",hash(theKey));

	strcpy(theKey,"DAN");
	printf("theKey is : %s\n",theKey);
	while(theKey[x] != '\0'){
		printf("theKey[%d] : %d\n",x,theKey[x]);
		printf("<< 1 : %d\n\n",(theKey[x]<<1));
		x++;
	}
	//printf("Daniel << 4 : %d\n",(theKey<<SHIFT));
return 0;
}
