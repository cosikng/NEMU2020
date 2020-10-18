#include "cpu/exec/helper.h"

make_helper(cbw)
{
    if (ops_decoded.is_operand_size_16)
    {
        if ((cpu.gpr[0]._8[0] & 0x80) != 0)
            cpu.gpr[0]._16 |= 0xff00;
        else
            cpu.gpr[0]._16 &= 0xff;
    }
    else
    {
        if ((cpu.gpr[0]._16 & 0x8000) != 0)
            cpu.eax |= 0xffff0000;
        else
            cpu.eax &= 0xffff;
    }
    return 1;
}