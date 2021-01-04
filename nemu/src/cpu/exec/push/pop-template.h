#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute()
{
    OPERAND_W(op_src, swaddr_read(cpu.esp, DATA_BYTE, 2));//SS
    cpu.esp += DATA_BYTE;
    print_asm_template1_n();
    return;
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
    make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
