/*************************************************************************
	> File Name: ge_list.h
	> Author:gpf 
	> Mail: g18720521413@163.com
	> Created Time: 2016年10月16日 星期日 16时40分26秒
 ************************************************************************/
#ifndef _GE_LIST_H
#define _GE_LIST_H

#define LIST_HEAD(name) \
	struct list_head name = { &name, &name }

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

struct list_head {
	struct list_head *next, *prev;
};

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

#define offset_userof(TYPE, MEMBER) ((user_u32) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({ \
        const typeof( ((type *)0)->member  ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offset_userof(type,member)  );})

extern void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next);

extern void list_add_tail(struct list_head *new, struct list_head *head);
#endif
