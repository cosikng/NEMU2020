#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute()
{

	swaddr_write(cpu.edi, 4, swaddr_read(cpu.esi, 4));
	if (cpu.eflags.DF == 0)
	{
		cpu.edi += DATA_BYTE;
		cpu.esi += DATA_BYTE;
	}
	else
	{
		cpu.edi -= DATA_BYTE;
		cpu.esi -= DATA_BYTE;
	}
	print_asm_template_s();
}

make_instr_helper(v)

#include "cpu/exec/template-end.h"
