#include "cpu/exec/helper.h"

make_helper(cwd)
{
    if (ops_decoded.is_operand_size_16)
    {
        if ((cpu.gpr[0]._16 & 0x8000) != 0)
            cpu.gpr[2]._16 = 0xffff;
        else
            cpu.gpr[2]._16 = 0;
    }
    else
    {
        if ((cpu.eax & 0x80000000) != 0)
            cpu.edx = 0xffffffff;
        else
            cpu.edx = 0;
    }
    return 1;
}