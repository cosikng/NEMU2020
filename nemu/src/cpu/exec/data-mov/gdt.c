#include "cpu/exec/helper.h"

make_helper(lgdt)
{
    uint32_t addr = op_src->val;
    printf("type:0x%x\n", op_src->type);
    cpu.GDTR.limit = swaddr_read(addr, 2);
    printf("0x%x\n", addr);
    cpu.GDTR.base_l = swaddr_read(addr + 2, 2);
    printf("0x%x\n", addr);
    cpu.GDTR.base_h = swaddr_read(addr + 4, 2);

    return 5;
}