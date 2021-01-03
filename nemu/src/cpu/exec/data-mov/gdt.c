#include "cpu/exec/helper.h"

make_helper(lgdt)
{
    cpu.GDTR.limit = swaddr_read(op_src->val, 2);
    cpu.GDTR.base_l = swaddr_read(op_src->val + 2, 2);
    cpu.GDTR.base_h = swaddr_read(op_src->val + 4, 2);

    return 5;
}