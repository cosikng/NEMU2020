#include "cpu/exec/template-start.h"

#define instr test



static void do_execute()
{
    DATA_TYPE result = op_dest->val & op_src->val;
    int l = (DATA_BYTE << 3) - 1;
    cpu.eflags.CF = 0;
    cpu.eflags.SF = result >> l;
    cpu.eflags.OF = 0;
    cpu.eflags.ZF = !result;
    result ^= result >> 4;
    result ^= result >> 2;
    result ^= result >> 1;
    cpu.eflags.PF = !(result & 1);
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)



#include "cpu/exec/template-end.h"
