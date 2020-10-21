#include "trap.h"
#include <string.h>

void f(char *a,char *b){
	int i;	
	for(i=0;b[i]!=0;i++){
		a[i]=b[i];
	}
	a[i]=0;
	return;
}

int main() {
	char s[]="Hello";
	char a[10];
	strcpy(a,s);
	nemu_assert(strcmp(a,"Hello")==0);
	return 0;
}
