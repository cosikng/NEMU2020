#include "trap.h"
#include <string.h>

int main() {
	char s[]="Hello";
	char a[10];
	int i;
	for(i=0;i<6;i++){
		a[i]=s[i];
	}

	nemu_assert(strcmp(a,"Hello")==0);
	return 0;
}
