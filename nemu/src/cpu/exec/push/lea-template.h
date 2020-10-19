#include "cpu/exec/template-start.h"

#define instr lea

static void do_execute()
{
    printf("0x%X\n",op_src->addr);
    OPERAND_W(op_dest, op_src->addr);
    print_asm_template2_n();
    return;
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
