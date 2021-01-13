#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)
make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)
make_helper(mov_r2s)
{
    uint32_t gdl, gdh;
    int data = swaddr_read(eip + 1, 1, 1); //CS
    cpu.Sreg[(data >> 3) & 7] = cpu.gpr[data & 7]._16;
    gdl = lnaddr_read((cpu.GDTR.base_h << 16) + cpu.GDTR.base_l + (cpu.gpr[data & 7]._16 >> 3) * 8, 4);
    gdh = lnaddr_read((cpu.GDTR.base_h << 16) + cpu.GDTR.base_l + (cpu.gpr[data & 7]._16 >> 3) * 8 + 4, 4);
    cpu.Sregcache[(data >> 3) & 7].limit = ((gdh & 0xf0000) << 12) + ((gdl & 0xffff) << 12);
    cpu.Sregcache[(data >> 3) & 7].base = (gdl >> 16) + ((gdh & 0xff) << 16) + (gdh & 0xff000000);
    if ((gdh >> 23) == 0)
        cpu.Sregcache[(data >> 3) & 7].limit >>= 12;
    return 2;
}
