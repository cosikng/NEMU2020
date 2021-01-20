#include "nemu.h"
#include <stdlib.h>
#include <time.h>

CPU_state cpu;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test()
{
	srand(time(0));
	uint32_t sample[8];
	uint32_t eip_sample = rand();
	cpu.eip = eip_sample;

	int i;
	for (i = R_EAX; i <= R_EDI; i++)
	{
		sample[i] = rand();
		reg_l(i) = sample[i];
		assert(reg_w(i) == (sample[i] & 0xffff));
	}

	assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
	assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
	assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
	assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
	assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
	assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
	assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
	assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

	assert(sample[R_EAX] == cpu.eax);
	assert(sample[R_ECX] == cpu.ecx);
	assert(sample[R_EDX] == cpu.edx);
	assert(sample[R_EBX] == cpu.ebx);
	assert(sample[R_ESP] == cpu.esp);
	assert(sample[R_EBP] == cpu.ebp);
	assert(sample[R_ESI] == cpu.esi);
	assert(sample[R_EDI] == cpu.edi);

	assert(eip_sample == cpu.eip);
}

void sreg_load(uint8_t sreg)
{
	Assert(cpu.CR0.protect_enable, "Not in protection mode");
	uint16_t index = cpu.Sreg[sreg] >> 3;
	uint32_t gdl, gdh;
	Assert(index * 8 < cpu.GDTR.limit, "segment selector out of limit");
	gdl = lnaddr_read((cpu.GDTR.base_h << 16) + cpu.GDTR.base_l + index * 8, 4);
	gdh = lnaddr_read((cpu.GDTR.base_h << 16) + cpu.GDTR.base_l + index * 8 + 4, 4);
	cpu.Sregcache[sreg].limit = ((gdh & 0xf0000) << 12) + ((gdl & 0xffff) << 12);
	cpu.Sregcache[sreg].base = (gdl >> 16) + ((gdh & 0xff) << 16) + (gdh & 0xff000000);
	if ((gdh >> 23) == 0)
		cpu.Sregcache[sreg].limit >>= 12;
}