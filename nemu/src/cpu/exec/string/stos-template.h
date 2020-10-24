#include "cpu/exec/template-start.h"

#define instr stos

static void do_execute()
{

	swaddr_write(cpu.edi, DATA_BYTE, REG(0));
	if (cpu.eflags.DF == 0)
	{
		cpu.edi += DATA_BYTE;
	}
	else
	{
		cpu.edi -= DATA_BYTE;
	}
	print_asm_template_s();
}

make_instr_helper(v)

#include "cpu/exec/template-end.h"
