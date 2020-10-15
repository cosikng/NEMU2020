#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "pop-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(pop_rm)



make_helper(pop_r16)
{
    int index = swaddr_read(cpu.eip, 1) & 7;
    cpu.gpr[index]._16 = swaddr_read(cpu.esp, 2);
    cpu.esp += 2;
    return 1;
}
make_helper(pop_r32)
{
    int index = swaddr_read(cpu.eip, 1) & 7;
    cpu.gpr[index]._32 = swaddr_read(cpu.esp, 4);
    cpu.esp += 4;
    return 1;
}

make_helper(pop_r)
{
    if(ops_decoded.is_operand_size_16)
    {
        return pop_r16(eip);
    }
    else return pop_r32(eip);
}