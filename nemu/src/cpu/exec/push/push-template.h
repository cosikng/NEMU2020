#include "cpu/exec/template-start.h"

#define instr push

static void do_execute()
{
	int byte;
	if (op_src->type == OP_TYPE_IMM)
		byte = 4;
	else
		byte = DATA_BYTE;
	cpu.esp -= byte;
	swaddr_write(cpu.esp, byte, op_src->val);
	print_asm_template1_n();
	return;
}

make_instr_helper(rm)
	make_instr_helper(i)

#include "cpu/exec/template-end.h"
