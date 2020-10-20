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
        cpu.eip = op_src->val;
        if (DATA_BYTE == 2)
        {
            cpu.eip &= 0xffff;
        }
        cpu.eip -= 2;
    }
    print_asm_template1_n();
    return;
}

make_instr_helper(si)
#if DATA_BYTE == 2 || DATA_BYTE == 4
    make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
