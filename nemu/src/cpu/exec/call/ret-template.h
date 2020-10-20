#include "cpu/exec/template-start.h"

#define instr ret

extern CPU_state cpu;

static void do_execute()
{
    cpu.eip = swaddr_read(cpu.esp, 4);
    cpu.esp += 4;
    cpu.esp += op_src->val;
    cpu.eip--;
    cpu.eip -= DATA_BYTE;
    print_asm_template1_n();
    return;
}

make_instr_helper(v)
make_instr_helper(i)

#include "cpu/exec/template-end.h"
