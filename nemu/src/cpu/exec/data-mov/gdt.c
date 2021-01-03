#include "cpu/exec/helper.h"

make_helper(lgdt)
{
    uint32_t addr = op_src->val;
    cpu.GDTR.limit = swaddr_read(addr, 2);
    cpu.GDTR.base_l = swaddr_read(addr + 2, 2);
    cpu.GDTR.base_h = swaddr_read(addr + 4, 2);

    return 5;
}