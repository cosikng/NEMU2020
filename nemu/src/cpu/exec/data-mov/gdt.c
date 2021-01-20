#include "cpu/exec/helper.h"

make_helper(lgdt)
{
    decode_rm_l(eip + 1);
    uint32_t addr = op_src->addr;
    cpu.GDTR.limit = lnaddr_read(addr, 2);
    cpu.GDTR.base_l = lnaddr_read(addr + 2, 2);
    cpu.GDTR.base_h = lnaddr_read(addr + 4, 2);

    return 6;
}

make_helper(lidt)
{
    decode_rm_l(eip + 1);
    uint32_t addr = op_src->addr;
    cpu.IDTR.limit = lnaddr_read(addr, 2);
    cpu.IDTR.base = lnaddr_read(addr + 2, 4);

    return 6;
}