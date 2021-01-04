#include "cpu/exec/helper.h"

make_helper(movs2r)
{
    uint8_t reg = swaddr_read(eip + 1, 1, 1) & 7;
    uint8_t s = (swaddr_read(eip + 1, 1, 1) >> 3) & 7;
    if (s == 0)
        cpu.gpr[reg]._32 = cpu.CR0.val;
    else if (s == 3)
        cpu.gpr[reg]._32 = cpu.CR3.val;
    return 2;
}

make_helper(movr2s)
{
    uint8_t reg = swaddr_read(eip + 1, 1, 1) & 7;
    uint8_t s = (swaddr_read(eip + 1, 1, 1) >> 3) & 7;
    if (s == 0)
        cpu.CR0.val = cpu.gpr[reg]._32;
    else if (s == 3)
        cpu.CR3.val = cpu.gpr[reg]._32;
    return 2;
}