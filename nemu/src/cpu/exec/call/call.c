#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "call-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "call-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "call-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */
make_helper_v(call_i)

    make_helper(call_rm_v)
{
    int len = ops_decoded.is_operand_size_16 ? decode_rm_w(eip + 1) : decode_rm_l(eip + 1);
    if (ops_decoded.is_operand_size_16)
    {
        swaddr_write(cpu.esp - 2, 2, cpu.eip + 1 + len);
        cpu.esp -= 2;
        cpu.eip = op_src->val & 0xffff;
        cpu.eip -= 1 + len;
    }
    else
    {
        swaddr_write(cpu.esp - 4, 4, cpu.eip + 1 + len);
        cpu.esp -= 4;
        cpu.eip = op_src->val;
        cpu.eip -= 1 + len;
    }
    return 1 + len;
}
