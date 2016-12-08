/*************************************************************************
	> File Name: ge_list.c
	> Author:gpf 
	> Mail: g18720521413@163.com
	> Created Time: 2016年10月16日 星期日 16时56分28秒
 ************************************************************************/

#include <ge_list.h>

void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

