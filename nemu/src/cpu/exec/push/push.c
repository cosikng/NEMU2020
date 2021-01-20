#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */
make_helper_v(push_r)

make_helper_v(push_rm)
make_helper_v(push_i)
make_helper_v(push_si)

make_helper(pushad)
{
    uint32_t regs[8];
    int i;
    for (i = 0; i < 8; i++)
        regs[i] = cpu.gpr[i]._32;
    for (i = 0; i < 8; i++)
    {
        reg_l(R_ESP) -= 4;
        swaddr_write(reg_l(R_ESP), 4, regs[i], R_SS);
    }
    return 1;
}
