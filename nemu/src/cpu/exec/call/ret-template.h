#include "cpu/exec/template-start.h"

#define instr ret

extern CPU_state cpu;

static void do_execute()
{
    cpu.eip = swaddr_read(cpu.esp, 4);
    cpu.esp += 4;
    cpu.eip--;
    return;
}

make_instr_helper(v)

#include "cpu/exec/template-end.h"
