#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute()
{

	uint32_t dest = REG(0);
	uint32_t src = swaddr_read(cpu.edi, DATA_BYTE);
	//printf("dest:0x%x,src:0x%x\n", dest, src);
	if (cpu.eflags.DF == 0)
	{
		cpu.edi += DATA_BYTE;
	}
	else
	{
		cpu.edi -= DATA_BYTE;
	}
	DATA_TYPE result = dest - src;
	int l = (DATA_BYTE << 3) - 1;
	cpu.eflags.CF = dest < src;
	cpu.eflags.SF = result >> l;
	int s1, s2;
	s1 = dest >> l;
	s2 = src >> l;
	cpu.eflags.OF = (s1 != s2 && s2 == cpu.eflags.SF);
	cpu.eflags.ZF = !result;
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.eflags.PF = !(result & 1);
	print_asm_template_s();
	sprintf(assembly,"scas");
}

make_instr_helper(v)

#include "cpu/exec/template-end.h"
