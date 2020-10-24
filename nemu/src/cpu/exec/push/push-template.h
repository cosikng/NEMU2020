#include "cpu/exec/template-start.h"

#define instr push

static void do_execute()
{
	int byte, data;
	if (op_src->type == OP_TYPE_IMM)
		byte = 4;
	else
		byte = DATA_BYTE;
	cpu.esp -= byte;
	data = op_src->val;
	swaddr_write(cpu.esp, byte, data);
	print_asm_template1_n();
	return;
}

make_instr_helper(rm)
	make_instr_helper(i)
		make_instr_helper(si)
#if DATA_BYTE == 2 || DATA_BYTE == 4
			make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
