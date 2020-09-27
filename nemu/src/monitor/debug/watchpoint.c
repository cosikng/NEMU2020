#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool()
{
	int i;
	for (i = 0; i < NR_WP; i++)
	{
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

WP *new_wp()
{
	if (free_ != NULL)
	{
		WP *p = free_;
		free_ = free_->next;
		p->next = head;
		head = p;
		return head;
	}
	assert(0);
}

void free_wp(int wp)
{
	if (wp < 0 || wp >= NR_WP)
		return;
	WP *now, *pre;
	now = head;
	pre = NULL;
	while (now != NULL && now->NO != wp)
	{
		pre = now;
		now = now->next;
	}
	if (now == NULL)
		return;
	pre->next = now->next;
	now->next = free_;
	free_ = now;
	return;
}

void print_p()
{
	WP *p = head;
	while (p != NULL)
	{
		printf("Watchpoint %d\t%s\n", p->NO, p->s);
		p = p->next;
	}
	return;
}
/* TODO: Implement the functionality of watchpoint */
