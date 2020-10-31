#include "cpu/exec/template-start.h"

#define instr call

static void do_execute()
{
	int byte = ops_decoded.is_operand_size_16 ? 2 : 4;
	cpu.esp -= byte;
	swaddr_write(cpu.esp, byte, cpu.eip + 5);
	int off = op_src->val;
	cpu.eip += off;
	print_asm_template1_n();
	return;
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
