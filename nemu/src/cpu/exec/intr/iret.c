#include "cpu/exec/helper.h"
#include <nemu.h>

/* for instruction encoding overloading */
int pop()
{
	uint32_t tmp = swaddr_read(reg_l(R_ESP), 4, R_SS);
	swaddr_write(reg_l(R_ESP), 4, 0, R_SS);
	reg_l(R_ESP) += 4;
	return tmp;
}
make_helper(iret)
{
	if (cpu.CR0.protect_enable == 0)
	{
		cpu.eip = pop();
		cpu.CS = pop();
		cpu.eflags._32 = pop();
	}
	else
	{
		cpu.eip = pop();
		cpu.CS = pop();
		cpu.eflags._32 = pop();
		sreg_load(R_CS);
	}
	print_asm("iret");
	return 0;
}
