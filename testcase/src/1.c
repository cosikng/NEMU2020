#include "trap.h"
#include <stdio.h>
#include <string.h>

int main() {
	char s[]="Hello";
	char a[10];
	sprintf(a,"%s",s);

	nemu_assert(strcmp(a,"Hello")==0);
	return 0;
}
