#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute()
{

	uint32_t data = swaddr_read(cpu.esi, DATA_BYTE, 3); //DS
	REG(0) = data;
	if (cpu.eflags.DF == 0)
	{
		cpu.esi += DATA_BYTE;
	}
	else
	{
		cpu.esi -= DATA_BYTE;
	}
	print_asm_template_s();
	sprintf(assembly, "lods");
}

make_instr_helper(v)

#include "cpu/exec/template-end.h"
