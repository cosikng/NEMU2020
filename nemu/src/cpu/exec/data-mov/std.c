#include "cpu/exec/helper.h"

make_helper(std)
{
    cpu.eflags.DF = 1;
    return 1;
}
make_helper(cld)
{
    cpu.eflags.DF = 0;
    return 1;
}