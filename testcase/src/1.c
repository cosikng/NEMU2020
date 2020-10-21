#include "trap.h"
#include <string.h>
#include <stdio.h>

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
	//memcpy(a,s,6);
	sprintf(a,"%s",s);
	nemu_assert(strcmp(a,"Hello")==0);
	return 0;
}
