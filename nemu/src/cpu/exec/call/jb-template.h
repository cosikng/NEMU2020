#include "cpu/exec/template-start.h"

#define instr jb

static void do_execute()
{
    int off = op_src->val;
    if (cpu.eflags.CF == 1)
        cpu.eip += off;
    print_asm_template1_n();
    return;
}

make_instr_helper(si)

#include "cpu/exec/template-end.h"
