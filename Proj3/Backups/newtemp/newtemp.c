#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int tempCtr=0;


char * newTemp(void){
   char* temp = malloc(20);
   sprintf(temp,"temp%d",tempCtr);
   tempCtr++;
   return temp;
}

int main(void){
   char * strval;

   strval = newTemp();
   printf("%s = 1 + 2\n",strval);

   strval = newTemp();
   printf("%s = 3 + 4\n",strval);

return 0;
}
