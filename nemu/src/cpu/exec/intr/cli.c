#include "cpu/exec/helper.h"

make_helper(cli)
{
    cpu.eflags.IF = 0;
    return 1;
}

make_helper(sti)
{
    cpu.eflags.IF = 1;
    return 1;
}