#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute()
{
	if (ops_decoded.is_operand_size_16)
		op_dest->size = 2;
	else
		op_dest->size = 4;
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2_n();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
