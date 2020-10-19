#include "cpu/exec/template-start.h"

#define instr setnp

static void do_execute()
{
	if (cpu.eflags.PF == 0)
		OPERAND_W(op_src, 1);
	else
		OPERAND_W(op_src, 0);

	print_asm_template1_n();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
