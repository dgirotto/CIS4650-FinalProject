#include <stdio.h>

int gcd(int,int);

int gdc(int u, int v){
	if(v==0){
		printf("Reached base case. Returning.\n");
		return;
	}
	else{
		printf("calling gcd: gcd(%d,%d)\n",v,(u%v));
		return gcd(v,(u%v));
	} 
}

int main(void){
	int x=0,y=0;
	scanf("%d%d",&x,&y);
	printf("%d\n", gcd(x,y));
	return 0;
}
