#include "cpu/exec/template-start.h"

#define instr jcxz

static void do_execute()
{
    int off = op_src->val;
    if (ops_decoded.is_operand_size_16)
    {
        if (cpu.gpr[1]._16 == 0)
            cpu.eip += off;
    }
    else
    {
        if (cpu.gpr[1]._32 == 0)
            cpu.eip += off;
    }

    print_asm_template1_n();
    return;
}

make_instr_helper(si)

#include "cpu/exec/template-end.h"
