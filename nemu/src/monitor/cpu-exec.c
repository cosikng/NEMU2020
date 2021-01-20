#include "monitor/monitor.h"
#include "cpu/helper.h"
#include <setjmp.h>

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 10

int nemu_state = STOP;

int exec(swaddr_t);

uint8_t i8259_query_intr(void);
void i8259_ack_intr(void);

typedef struct watchpoint
{
	int NO;
	int sum;
	char s[50];
	struct watchpoint *next;

} WP;

extern int pp(char *args);

extern WP *gethead(void);

char assembly[80];
char asm_buf[128];

/* Used with exception handling. */
jmp_buf jbuf;

static inline void push(uint32_t val)
{
	reg_l(R_ESP) -= 4;
	swaddr_write(reg_l(R_ESP), 4, val, R_SS);
	return;
}

void raise_intr(uint8_t NO)
{
	GATE_descriptor idt;
	Assert(NO * 8 <= cpu.IDTR.limit, "Interrupt number exceeded.\n");
	lnaddr_t addr = cpu.IDTR.base + NO * 8;
	idt.first_part = lnaddr_read(addr, 4);
	idt.second_part = lnaddr_read(addr + 4, 4);
	push(cpu.eflags._32);
	push(cpu.CS);
	push(cpu.eip);
	cpu.CS = idt.segment;
	Assert(((cpu.CS >> 3) << 3) <= cpu.GDTR.limit, "segment out limit %d, %d", ((cpu.CS >> 3) << 3), cpu.GDTR.limit);
	sreg_load(R_CS);
	cpu.eip = idt.offset_15_0 + (idt.offset_31_16 << 16);
	longjmp(jbuf, 1);
}

void print_bin_instr(swaddr_t eip, int len)
{
	int i;
	int l = sprintf(asm_buf, "%8x:   ", eip);
	for (i = 0; i < len; i++)
	{
		l += sprintf(asm_buf + l, "%02x ", instr_fetch(eip + i, 1));
	}
	sprintf(asm_buf + l, "%*.s", 50 - (12 + 3 * len), "");
}

/* This function will be called when an `int3' instruction is being executed. */
void do_int3()
{
	printf("\nHit breakpoint at eip = 0x%08x\n", cpu.eip);
	nemu_state = STOP;
}

/* Simulate how the CPU works. */
void cpu_exec(volatile uint32_t n)
{
	if (nemu_state == END)
	{
		printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
		return;
	}
	nemu_state = RUNNING;

#ifdef DEBUG
	volatile uint32_t n_temp = n;
#endif

	setjmp(jbuf);

	for (; n > 0; n--)
	{
#ifdef DEBUG
		swaddr_t eip_temp = cpu.eip;
		if ((n & 0xffff) == 0)
		{
			/* Output some dots while executing the program. */
			fputc('.', stderr);
		}
#endif

		/* Execute one instruction, including instruction fetch,
		 * instruction decode, and the actual execution. */
		int instr_len = exec(cpu.eip);

		cpu.eip += instr_len;

#ifdef DEBUG
		print_bin_instr(eip_temp, instr_len);
		strcat(asm_buf, assembly);
		Log_write("%s\n", asm_buf);
		if (n_temp < MAX_INSTR_TO_PRINT)
		{
			printf("%s\n", asm_buf);
		}
#endif

		/* TODO: check watchpoints here. */
		WP *p = gethead();
		while (p != NULL)
		{
			int now = pp(p->s);
			if (p->sum != now)
			{
				do_int3();
				printf("Watchpoint %d (%s) changed from %d to %d\n", p->NO, p->s, p->sum, now);
				p->sum = now;
			}
			p = p->next;
		}

#ifdef HAS_DEVICE
		extern void device_update();
		device_update();
#endif

		if (nemu_state != RUNNING)
		{
			return;
		}
		if (cpu.INTR & cpu.eflags.IF)
		{
			uint32_t intr_no = i8259_query_intr();
			i8259_ack_intr();
			raise_intr(intr_no);
		}
	}

	if (nemu_state == RUNNING)
	{
		nemu_state = STOP;
	}
}
