#include "cpu/exec/template-start.h"

#define instr push

extern CPU_state cpu;

static void do_execute()
{
	int index = swaddr_read(cpu.eip, 1) - 0x50;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.gpr[index]._32);
	print_asm_template1();
	return;
}

make_instr_helper(v)

#include "cpu/exec/template-end.h"
