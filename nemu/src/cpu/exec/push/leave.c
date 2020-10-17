#include "cpu/exec/helper.h"

make_helper(leave)
{
    if (ops_decoded.is_operand_size_16)
    {
        cpu.gpr[4]._16 = cpu.gpr[5]._16;
        cpu.gpr[5]._16 = swaddr_read(cpu.gpr[4]._16, 2);
        cpu.gpr[4]._16 += 2;
    }
    else
    {
        cpu.esp = cpu.ebp;
        cpu.ebp = swaddr_read(cpu.esp, 4);
        cpu.esp += 4;
    }
    return 1;
}