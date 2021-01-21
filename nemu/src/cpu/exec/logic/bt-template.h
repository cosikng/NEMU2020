#include "cpu/exec/template-start.h"

#define instr bt

static void do_execute()
{
	DATA_TYPE result = ((op_dest->val << 1) >> (op_src->val % 32)) & 1;
	cpu.eflags.CF = result;

	print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
