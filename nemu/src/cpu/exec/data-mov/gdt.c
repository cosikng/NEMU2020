#include "cpu/exec/helper.h"

make_helper(lgdt)
{
    uint32_t len;
    len = decode_rm_l(eip + 1);
    uint32_t addr = op_src->addr;
    cpu.GDTR.limit = lnaddr_read(addr, 2);
    cpu.GDTR.base_l = lnaddr_read(addr + 2, 2);
    cpu.GDTR.base_h = lnaddr_read(addr + 4, 2);
    print_asm("lgdt");
    return len + 1;
}

make_helper(lidt)
{
    uint32_t len;
    len = decode_rm_l(eip + 1);
    uint32_t addr = op_src->addr;
    cpu.IDTR.limit = lnaddr_read(addr, 2);
    cpu.IDTR.base = lnaddr_read(addr + 2, 4);
    print_asm("lidt");
    return len + 1;
}