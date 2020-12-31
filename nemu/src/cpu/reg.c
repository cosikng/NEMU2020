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

	int i, j;
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

	cpu.cache1.b = 6;
	cpu.cache1.E = 8;
	cpu.cache1.s = 7;
	cpu.cache1.sets = (struct set *)malloc(sizeof(struct set) * (1 << (cpu.cache1.s)));
	for (i = 0; i < (1 << (cpu.cache1.s)); i++)
	{
		cpu.cache1.sets[i].blocks = (struct block *)malloc(sizeof(struct block) * cpu.cache1.E);
		for (j = 0; j < cpu.cache1.E; j++)
		{
			cpu.cache1.sets[i].blocks[j].buf = (uint8_t *)malloc(sizeof(uint8_t) * (1 << (cpu.cache1.b)));
			cpu.cache1.sets[i].blocks[j].valid = false;
		}
	}
}
