#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute()
{
    if (op_src->type == OP_TYPE_IMM)
    {
        int off = op_src->val;
        cpu.eip += off;
    }
    print_asm_template1_n();
    return;
}

make_instr_helper(si)

#include "cpu/exec/template-end.h"
