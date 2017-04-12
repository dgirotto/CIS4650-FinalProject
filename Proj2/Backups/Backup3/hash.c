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
