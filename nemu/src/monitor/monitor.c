#include "nemu.h"
#include <stdlib.h>

#define ENTRY_START 0x100000

extern uint8_t entry[];
extern uint32_t entry_len;
extern char *exec_file;

void load_elf_tables(int, char *[]);
void init_regex();
void init_wp_pool();
void init_ddr3();

FILE *log_fp = NULL;

static void init_log()
{
	log_fp = fopen("log.txt", "w");
	Assert(log_fp, "Can not open 'log.txt'");
}

static void welcome()
{
	printf("Welcome to NEMU!\nThe executable is %s.\nFor help, type \"help\"\n",
		   exec_file);
}

void init_monitor(int argc, char *argv[])
{
	/* Perform some global initialization */

	/* Open the log file. */
	init_log();

	/* Load the string table and symbol table from the ELF file for future use. */
	load_elf_tables(argc, argv);

	/* Compile the regular expressions. */
	init_regex();

	/* Initialize the watchpoint pool. */
	init_wp_pool();

	/* Display welcome message. */
	welcome();
}

#ifdef USE_RAMDISK
static void init_ramdisk()
{
	int ret;
	const int ramdisk_max_size = 0xa0000;
	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	Assert(file_size < ramdisk_max_size, "file size(%zd) too large", file_size);

	fseek(fp, 0, SEEK_SET);
	ret = fread(hwa_to_va(0), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}
#endif

static void load_entry()
{
	int ret;
	FILE *fp = fopen("entry", "rb");
	Assert(fp, "Can not open 'entry'");

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	ret = fread(hwa_to_va(ENTRY_START), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}

void restart()
{
	int i, j;
	/* Perform some initialization to restart a program */
#ifdef USE_RAMDISK
	/* Read the file with name `argv[1]' into ramdisk. */
	init_ramdisk();
#endif

	/* Read the entry code into memory. */
	load_entry();

	/* Set the initial instruction pointer. */
	cpu.eip = ENTRY_START;

	cpu.eflags._32 = 0x2;

	cpu.cache1.b = 6;
	cpu.cache1.E = 8;
	cpu.cache1.s = 7;
	cpu.cache1.hit = 0;
	cpu.cache1.miss = 0;
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

	/* Initialize DRAM. */
	init_ddr3();
}
