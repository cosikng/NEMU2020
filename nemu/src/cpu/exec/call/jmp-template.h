#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute()
{
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip + 5);
	int off = op_src->val;
	cpu.eip += off;
	print_asm_template1_n();
	return;
}

make_instr_helper(si)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
