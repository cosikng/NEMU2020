#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jmp-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(jmp_si)
    make_helper_v(jmp_rm)

        make_helper(ljmp)
{
    cpu.CS = swaddr_read(eip + 5, 2, 1) >> 3;
    cpu.eip = swaddr_read(eip + 1, 4, 1) - 7;
    return 7;
}
