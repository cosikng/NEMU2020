#include "cpu/exec/template-start.h"

#define instr call

static void do_execute()
{
	cpu.esp -= op_src->size;
	printf("%d\n",(int)op_src->size);
	swaddr_write(cpu.esp, op_src->size, cpu.eip + 1 + op_src->size);
	int off = op_src->val;
	cpu.eip += off;
	if (ops_decoded.is_operand_size_16)
		cpu.eip &= 0xffff;
	print_asm_template1_n();
	return;
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
