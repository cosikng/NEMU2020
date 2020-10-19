#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute()
{
	if (op_src->size == 1)
	{
		if ((op_src->val & 0x80) != 0)
			op_src->val |= 0xffffff00;
	}
	else
	{
		if ((op_src->val & 0x8000) != 0)
			op_src->val |= 0xffff0000;
	}
	if (ops_decoded.is_operand_size_16)
		cpu.gpr[op_dest->reg]._16 = op_src->val;
	else
		cpu.gpr[op_dest->reg]._32 = op_src->val;
	print_asm_template2_n();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
