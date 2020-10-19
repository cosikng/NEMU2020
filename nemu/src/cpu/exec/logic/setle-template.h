#include "cpu/exec/template-start.h"

#define instr setle

static void do_execute()
{
	if (cpu.eflags.SF != cpu.eflags.OF || cpu.eflags.ZF == 1)
		OPERAND_W(op_src, 1);
	else
		OPERAND_W(op_src, 0);

	print_asm_template1_n();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
