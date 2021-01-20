#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "pop-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(pop_rm)
make_helper_v(pop_r)

make_helper(popad)
{
    int i;
    for (i = 7; i >= 0; i--)
    {
        if (i == R_ESP)
        {
            cpu.esp += 4;
            continue;
        }
        cpu.gpr[i]._32 = swaddr_read(reg_l(R_ESP), 4, R_SS);
        cpu.esp += 4;
    }
    return 1;
}
