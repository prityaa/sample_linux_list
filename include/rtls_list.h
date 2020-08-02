#ifndef __RTLS_LIST_H__
#define __RTLS_LIST_H__

struct list_head {
	struct list_head *next, *prev;
};


/**
 * Returns a pointer to the container of this list element.
 *
 * Example:
 * struct foo* f;
 * f = container_of(&foo->entry, struct foo, entry);
 * assert(f == foo);
 *
 * @param ptr Pointer to the struct list_head.
 * @param type Data type of the list element.
 * @param member Member name of the struct list_head field in the list element.
 * @return A pointer to the data struct containing the list head.
 */

#define container_of(ptr, type, member) \
	(type *)((char *)(ptr) - (char *) &((type *)0)->member)

/**
 * list_entry - get the struct for this entry
 * @ptr:        the &struct list_head pointer.
 * @type:       the type of the struct this is embedded in.
 * @member:     the name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * list_for_each        -       iterate over a list
 * @pos:        the &struct list_head to use as a loop cursor.
 * @head:       the head for your list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

static void INIT_LIST_HEAD(struct list_head *list)
{       
	list->next = list->prev = list; 
}

static void __list_add(struct list_head *entry,
		struct list_head *prev, struct list_head *next)
{
	next->prev = entry;
	entry->next = next;
	entry->prev = prev;
	prev->next = entry;
}

static void list_add(struct list_head *entry, struct list_head *head)
{
	__list_add(entry, head, head->next);
}

static inline void
__list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * Remove the element from the list it is in. Using this function will reset
 * the pointers to/from this element so it is removed from the list. It does
 * NOT free the element itself or manipulate it otherwise.
 *
 * Using list_del on a pure list head (like in the example at the top of
 * this file) will NOT remove the first element from
 * the list but rather reset the list as empty list.
 *
 * Example:
 * list_del(&foo->entry);
 *
 * @param entry The element to remove.
 */
static inline void
list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

#endif
