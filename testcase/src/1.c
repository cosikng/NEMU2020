#include "trap.h"
/*#include <string.h>
#include <stdio.h>

void f(char *a,char *b){
	int i;	
	for(i=0;b[i]!=0;i++){
		a[i]=b[i];
	}
	a[i]=0;
	return;
}*/

int main() {
	/*char s[]="ab";
	char a[10];
	memcpy(a,s,3);
	//snprintf(a,3,"%s",s);
	nemu_assert(strcmp(a,s)==0);*/
	int a=-2;
	unsigned int b=-2;
	nemu_assert((a>>1)==-1);
	nemu_assert((b>>1)==2147483647);
	return 0;
}
