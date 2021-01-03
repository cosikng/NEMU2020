#include "cpu/exec/helper.h"

make_helper(movs2r)
{
    uint8_t reg = swaddr_read(eip + 1, 1) & 7;
    cpu.gpr[reg]._32 = cpu.CR0.val;
    return 2;
}

make_helper(movr2s)
{
    uint8_t reg = swaddr_read(eip + 1, 1) & 7;
    cpu.CR0.val = cpu.gpr[reg]._32;
    return 2;
}