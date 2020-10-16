#include "cpu/exec/template-start.h"

#define instr call

static void do_execute()
{
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip + 5);
	int off = op_src->val;
	cpu.eip += off;
	print_asm_template1();
	return;
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
