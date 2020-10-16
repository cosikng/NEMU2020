#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute()
{
    swaddr_write(op_src->val, DATA_BYTE, swaddr_read(cpu.esp, DATA_BYTE));
    cpu.esp += DATA_BYTE;
    print_asm_template1();
    return;
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
