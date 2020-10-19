#include "trap.h"

int main() {
	short c=-1;
	unsigned int a=c;

	nemu_assert(a == 1);

	return 0;
}
