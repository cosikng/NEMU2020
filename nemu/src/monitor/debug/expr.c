#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

#define NUM 0
#define OPT 1

void tod(char *s, int len); /*十六进制和寄存器转换为十进制*/

char dnum[50]; /*保存十进制数的字符串*/

extern CPU_state cpu;

enum
{
	NOTYPE = 256,
	EQ

	/* TODO: Add more token types */

};

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE}, // spaces
	{"\\+", OPT},	// plus
	{"==", OPT},	// equal
	{"\\*", OPT},	//mul
	{"/", OPT},
	{"-", OPT},
	{"\\(", OPT},
	{"\\)", OPT},
	{"\\&\\&", OPT},
	{"\\|\\|", OPT},
	{"!=", OPT},
	{"!", OPT},
	{"0x[0123456789abcdef]+", NUM},
	{"\\$[a-z]{3}", NUM},
	{"[0-9]+", NUM}};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token
{
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				switch (rules[i].token_type)
				{
				case OPT:
					tokens[nr_token].type = OPT;
					/*	||	->	a
					&&	->	b
					!=	->	c
					==	->	d
					*/
					if (substr_len > 1)
					{
						if (strncmp(substr_start, "||", 2) == 0)
						{
							strncpy(tokens[nr_token].str, "a", 2);
						}
						else if (strncmp(substr_start, "&&", 2) == 0)
						{
							strncpy(tokens[nr_token].str, "b", 2);
						}
						else if (strncmp(substr_start, "!=", 2) == 0)
						{
							strncpy(tokens[nr_token].str, "c", 2);
						}
						else if (strncmp(substr_start, "==", 2) == 0)
						{
							strncpy(tokens[nr_token].str, "d", 2);
						}
					}
					else
					{
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len] = 0;
					}
					nr_token++;
					break;
				case NUM:
					tokens[nr_token].type = NUM;
					tod(substr_start, substr_len);
					strncpy(tokens[nr_token].str, dnum, strlen(dnum));
					tokens[nr_token].str[strlen(dnum)] = 0;
					nr_token++;
					break;
				case NOTYPE:
					break;

				default:
					break;
				}

				break;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

void tod(char *s, int len)
{
	if (strncmp(s, "0x", 2) == 0)
	{
		int i, num = 0;
		for (i = 2; i < len; i++)
		{
			if (s[i] >= 'a' && s[i] <= 'f')
			{
				num = num * 16 + s[i] - 'a' + 10;
			}
			else
			{
				num = num * 10 + s[i] - '0';
			}
		}
		for (i = 0; num != 0; i++)
		{
			int j;
			for (j = i; j > 0; j--)
			{
				dnum[j] = dnum[j - 1];
			}
			dnum[0] = num % 10 + '0';
			num /= 10;
		}
		dnum[i] = 0;
	}
	else if (s[0] == '$')
	{
		int n = 0;
		if (strncmp(s, "$eax", 4) == 0)
		{
			n = cpu.eax;
		}
		else if (strncmp(s, "$ecx", 4) == 0)
		{
			n = cpu.ecx;
		}
		else if (strncmp(s, "$edx", 4) == 0)
		{
			n = cpu.edx;
		}
		else if (strncmp(s, "$ebx", 4) == 0)
		{
			n = cpu.ebx;
		}
		else if (strncmp(s, "$esp", 4) == 0)
		{
			n = cpu.esp;
		}
		else if (strncmp(s, "$ebp", 4) == 0)
		{
			n = cpu.ebp;
		}
		else if (strncmp(s, "$esi", 4) == 0)
		{
			n = cpu.esi;
		}
		else if (strncmp(s, "$edi", 4) == 0)
		{
			n = cpu.edi;
		}
		else if (strncmp(s, "$eip", 4) == 0)
		{
			n = cpu.eip;
		}
		int i;
		for (i = 0; n != 0; i++)
		{
			int j;
			for (j = i; j > 0; j--)
			{
				dnum[j] = dnum[j - 1];
			}
			dnum[0] = n % 10 + '0';
			n /= 10;
		}
		dnum[i] = 0;
	}
	else
	{
		strncpy(dnum, s, len);
		dnum[len] = 0;
	}
	return;
}

uint32_t expr(char *e, bool *success)
{
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	//panic("please implement me");
	return 0;
}
