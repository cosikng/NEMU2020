#include "cpu/exec/helper.h"

void raise_intr(uint8_t NO);

make_helper(intr)
{
    uint8_t no = instr_fetch(cpu.eip + 1, 1);
    print_asm("int %x", no);
    raise_intr(no);
    return 2;
}