#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute()
{
    if (op_src->type == OP_TYPE_IMM)
    {
        int off = op_src->val;
        cpu.eip += off;
    }
    else
    {
        int off = 0;
        cpu.eip = op_src->val;
        if (DATA_BYTE == 2)
        {
            cpu.eip &= 0xffff;
        }
        if (ops_decoded.is_operand_size_16)
        {
            off = 2 + decode_rm_w(cpu.eip + 1);
        }
        else
        {
            off = 1 + decode_rm_l(cpu.eip + 1);
        }
        cpu.eip -= off;
    }
    print_asm_template1_n();
    return;
}

make_instr_helper(si)
#if DATA_BYTE == 2 || DATA_BYTE == 4
    make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
