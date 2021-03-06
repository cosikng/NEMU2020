#include "cpu/exec/template-start.h"

#define instr sar

static void do_execute () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE_S dest = op_dest->val;

	uint8_t count = src & 0x1f;
	dest >>= count;
	int l = (DATA_BYTE << 3) - 1;
	cpu.eflags.CF = ((op_dest->val << 1) >> count) & 1;
	if(count == 1) cpu.eflags.OF = 0;
	cpu.eflags.SF = dest >> l;
	cpu.eflags.ZF = !dest;
	OPERAND_W(op_dest, dest);
	dest ^= dest >> 4;
	dest ^= dest >> 2;
	dest ^= dest >> 1;
	cpu.eflags.PF = !(dest & 1);

	/* TODO: Update EFLAGS. */
	//panic("please implement me");

	print_asm_template2();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"
