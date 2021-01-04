#include "cpu/exec/helper.h"

#define instr leave

make_helper(leave)
{
    cpu.esp = cpu.ebp;
    if (ops_decoded.is_operand_size_16)
    {
        cpu.gpr[5]._16 = swaddr_read(cpu.gpr[4]._16, 2, 2); //SS
        cpu.gpr[4]._16 += 2;
    }
    else
    {

        cpu.ebp = swaddr_read(cpu.esp, 4, 2);
        cpu.esp += 4;
    }
    print_asm_template_s();
    return 1;
}