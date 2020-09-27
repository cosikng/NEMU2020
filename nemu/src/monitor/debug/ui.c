#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

extern CPU_state cpu;

extern uint8_t *hw_mem;

extern bool make_token(char *e);

extern int nr_token;

extern void print_p(void);

int aton(char *s);

int class(char c);

int cac(int n1, int n2, char opt);

int pp(char *args);

typedef struct token
{
	int type;
	char str[32];
} Token;

extern Token tokens[32];

/* We use the `readline' library to provide more flexibility to read from stdin. */
char *rl_gets()
{
	static char *line_read = NULL;

	if (line_read)
	{
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read)
	{
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args)
{
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args)
{
	return -1;
}
static int cmd_si(char *args)
{
	int n = 0;
	if (args == NULL)
	{
		n = 1;
	}
	else
	{
		int i;
		for (i = 0; args[i] != 0; i++)
		{
			n = n * 10 + args[i] - '0';
		}
	}
	cpu_exec(n);
	return 0;
}
static int cmd_info(char *args)
{
	char s[] = "eaxecxedxebxespebpesiedi";
	if (*args == 'r')
	{
		int i;
		for (i = 0; i < 8; i++)
		{
			printf("%c%c%c\t0x%x\n", s[i * 3], s[i * 3 + 1], s[i * 3 + 2], cpu.gpr[i]._32);
		}
		printf("eip\t0x%x\n", cpu.eip);
	}
	else if (*args == 'w')
	{
		print_p();
	}
	return 0;
}
static int cmd_x(char *args)
{
	int n = 0;
	int addr = 0;
	int i;
	for (i = 0; args[i] != ' '; i++)
	{
		n = n * 10 + args[i] - '0';
	}
	for (i += 3; args[i] != 0; i++)
	{
		if (args[i] >= 'a' && args[i] <= 'f')
		{
			addr = addr * 16 + 10 + args[i] - 'a';
		}
		else
		{
			addr = addr * 16 + args[i] - '0';
		}
	}
	printf("Addr\t\tData\n");
	for (i = 0; i < n; i++)
	{
		printf("0x%X\t0x%08x\n", addr + i * 4, *((unsigned int *)(addr + i * 4 + hw_mem)));
	}
	return 0;
}
static int cmd_p(char *args)
{
	printf("%d\n", pp(args));
	return 0;
}

extern WP *new_wp();
static int cmd_w(char *args)
{
	WP *p = new_wp();
	strcpy(p->s, args);
	p->sum = pp(p->s);
	printf("Watchpoint %d\t%s = %d\n", p->NO, p->s, p->sum);
	return 0;
}

static int cmd_d(char *args)
{
	free_wp(atoi(args));
	return 0;
}

static int cmd_help(char *args);

static struct
{
	char *name;
	char *description;
	int (*handler)(char *);
} cmd_table[] = {
	{"help", "Display informations about all supported commands", cmd_help},
	{"c", "Continue the execution of the program", cmd_c},
	{"q", "Exit NEMU", cmd_q},
	{"si", "Single step", cmd_si},
	{"info", "Info", cmd_info},
	{"x", "Scan RAM", cmd_x},
	{"p", "Test", cmd_p},
	{"w", "Points", cmd_w},
	{"d", "Delete points", cmd_d}

	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args)
{
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if (arg == NULL)
	{
		/* no argument given */
		for (i = 0; i < NR_CMD; i++)
		{
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else
	{
		for (i = 0; i < NR_CMD; i++)
		{
			if (strcmp(arg, cmd_table[i].name) == 0)
			{
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

int aton(char *s)
{
	int n = 0;
	int i;
	for (i = 0; s[i] != 0; i++)
	{
		n = n * 10 + s[i] - '0';
	}
	return n;
}

int class(char c)
{
	if (c == '+' || c == '-')
	{
		return 4;
	}
	else if (c == '*' || c == '/')
	{
		return 5;
	}
	else if (c == '(' || c == ')')
	{
		return 0;
	}
	else if (c == '!' || c == 'p') /*	*ptr	->	pptr	*/
	{
		return 6;
	}
	else if (c == 'c' || c == 'd')
	{
		return 3;
	}
	else if (c == 'a')
	{
		return 1;
	}
	else if (c == 'b')
	{
		return 2;
	}
	return -1;
}

int cac(int n1, int n2, char opt)
{
	if (opt == '+')
	{
		return n1 + n2;
	}
	else if (opt == '-')
	{
		return n1 - n2;
	}
	else if (opt == '*')
	{
		return n1 * n2;
	}
	else if (opt == '/')
	{
		return n1 / n2;
	}
	else if (opt == 'a')
	{
		return n1 != 0 || n2 != 0 ? 1 : 0;
	}
	else if (opt == 'b')
	{
		return n1 == 1 && n2 == 1 ? 1 : 0;
	}
	else if (opt == 'c')
	{
		return n1 != n2 ? 1 : 0;
	}
	else if (opt == 'd')
	{
		return n1 == n2 ? 1 : 0;
	}
	else if (opt == '!')
	{
		return !n2;
	}
	else if (opt == 'p')
	{
		return *((unsigned int *)(hw_mem + n2));
	}
	else
	{
		return 0;
	}
}

int pp(char *args)
{
	bool a = true;
	expr(args, &a);
	int num[20];
	char opt[20];
	int pn = 0, po = 0;
	if (a)
	{
		int i;
		/*负号、指针解引用预处理*/
		for (i = 0; i < nr_token; i++)
		{
			if (tokens[i].str[0] == '-')
			{
				if (i == 0 || (tokens[i - 1].type == 1 && tokens[i - 1].str[0] != ')'))
				{
					int j;
					nr_token += 2;
					for (j = nr_token - 1; j > i + 1; j--)
					{
						tokens[j] = tokens[j - 2];
					}
					i += 2;
					tokens[i - 2].type = 1;
					strcpy(tokens[i - 2].str, "(");
					tokens[i - 1].type = 0;
					strcpy(tokens[i - 1].str, "0");
					int kh = 0;
					//printf("a\n");
					for (j = i + 1; j < nr_token && (kh == 1 || (tokens[j].str[0] != '+' && tokens[j].str[0] != '-') || ((tokens[j].str[0] == '+' || tokens[j].str[0] == '-') && (tokens[j - 1].type == 1 && (tokens[j - 1].str[0] != '(' && tokens[j - 1].str[0] != ')')))); j++)
					{
						if (tokens[j].str[0] == '(')
						{
							kh++;
						}
						else if (tokens[j].str[0] == ')' && kh != 0)
						{
							kh--;
						}
					}
					nr_token++;
					for (kh = nr_token - 1; kh > j; kh--)
					{
						tokens[kh] = tokens[kh - 1];
					}
					tokens[j].type = 1;
					strcpy(tokens[j].str, ")");
					/*int m;
					for (m = 0; m < nr_token; m++)
						printf("%c\t", tokens[m].str[0]);
					printf("\n");*/
				}
			}
			else if (tokens[i].str[0] == '*')
			{
				if (i == 0 || (tokens[i - 1].type == 1 && tokens[i - 1].str[0] != ')'))
				{
					tokens[i].str[0] = 'p';
				}
			}
		}
		/*负号、指针解引用预处理结束*/

		for (i = 0; i < nr_token;)
		{
			if (tokens[i].type == 0)
			{
				num[pn++] = aton(tokens[i].str);
			}
			else if (tokens[i].type == 1)
			{
				if (tokens[i].str[0] == '(')
				{
					opt[po++] = tokens[i].str[0];
					i++;
					continue;
				}
				for (; po > 0 && class(opt[po - 1]) >= class(tokens[i].str[0]); po--)
				{
					if (opt[po - 1] == '(')
					{
						po--;
						goto con;
					}
					if (opt[po - 1] == '!' || opt[po - 1] == 'p') //单目运算符特殊处理
					{
						int n = num[--pn];
						num[pn++] = cac(0, n, opt[po - 1]);
					}
					else
					{
						int n2 = num[--pn];
						int n1 = num[--pn];
						num[pn++] = cac(n1, n2, opt[po - 1]);
					}
				}
				opt[po++] = tokens[i].str[0];
			}
		con:
			i++;
		}
		for (; pn > 1 || po > 0;)
		{
			if (opt[po - 1] == '!' || opt[po - 1] == 'p') //单目运算符特殊处理
			{
				int n = num[--pn];
				num[pn++] = cac(0, n, opt[--po]);
			}
			else
			{
				int n2 = num[--pn];
				int n1 = num[--pn];
				num[pn++] = cac(n1, n2, opt[--po]);
			}
		}
		if (pn != 1 || po != 0)
		{
			printf("Error\n");
		}
	}
	return num[0];
}

void ui_mainloop()
{
	while (1)
	{
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if (cmd == NULL)
		{
			continue;
		}

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if (args >= str_end)
		{
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for (i = 0; i < NR_CMD; i++)
		{
			if (strcmp(cmd, cmd_table[i].name) == 0)
			{
				if (cmd_table[i].handler(args) < 0)
				{
					return;
				}
				break;
			}
		}

		if (i == NR_CMD)
		{
			printf("Unknown command '%s'\n", cmd);
		}
	}
}
