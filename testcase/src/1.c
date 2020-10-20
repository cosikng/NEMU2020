#include "trap.h"
#include <string.h>
#include <stdio.h>

int main() {
	//char s[]="Hello";
	char a[100];
	sprintf(a,"%d",10);
	nemu_assert(strcmp(a,"10")==0);
	return 0;
}
