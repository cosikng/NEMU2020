#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute()
{

	uint32_t data = swaddr_read(cpu.edi, DATA_BYTE);
	REG(0) = data;
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
