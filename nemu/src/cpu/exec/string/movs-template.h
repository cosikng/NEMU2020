#include "cpu/exec/template-start.h"

#define instr movs
extern uint8_t *hw_mem;

static void do_execute()
{
	uint32_t x=(uint32_t)hw_mem;
	printf("\n0x%x\n",x);
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
