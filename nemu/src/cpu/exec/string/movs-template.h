#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute()
{
	swaddr_write(cpu.edi, DATA_BYTE, swaddr_read(cpu.esi, DATA_BYTE, 3), 0); //DS->ES
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
	sprintf(assembly, "movs");
}

make_instr_helper(v)

#include "cpu/exec/template-end.h"
