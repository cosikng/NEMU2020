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
    uint32_t gdl, gdh;
    cpu.CS = swaddr_read(eip + 5, 2, 1);
    gdl = lnaddr_read((cpu.GDTR.base_h << 16) + cpu.GDTR.base_l + (cpu.CS >> 3) * 8, 4);
    gdh = lnaddr_read((cpu.GDTR.base_h << 16) + cpu.GDTR.base_l + (cpu.CS >> 3) * 8 + 4, 4);
    cpu.CSlimit = ((gdh & 0xf0000) << 12) + ((gdl & 0xffff) << 12);
    cpu.CSbase = (gdl >> 16) + ((gdh & 0xff) << 16) + (gdh & 0xff000000);
    if((gdh>>23)==0) assert(0);
    cpu.eip = swaddr_read(eip + 1, 4, 1) - 7;
    return 7;
}
