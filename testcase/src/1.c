#include "trap.h"

int main() {
	char c=-1;
	int a=c;

	nemu_assert(a == -1);

	return 0;
}
