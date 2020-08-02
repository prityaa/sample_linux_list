#include <stdio.h>
#include <stdlib.h>
#include <rtls_rsc.h>
//#include <rtls_list.h>

#define DEBUG_LIST
#include <dbg.h>

/* head of the mem node */
static struct list_head mem_list;

void *rtls_malloc(int size)
{
	struct rtls_mem *mem;
	mem = (struct rtls_mem *)malloc(sizeof(*mem));
	void *temp = malloc(size);
	struct list_head *list = &mem->list;	

	dbg_list("&mem_list = %p\n", &mem_list);
	mem->addr = temp;
	mem->size = size;
	
	dbg_list("mem %p temp = %p\n", mem, temp);
	if (temp) {
		list_add(&(mem->list), &mem_list);
	}

	return temp;
}

static void print_list_mem(struct list_head *mem_list)
{
        struct list_head *tmp;
        struct rtls_mem *mem;

        dbg_list("listing mem\n");
        list_for_each(tmp, mem_list) {

                mem = list_entry(tmp, struct rtls_mem, list);
 
                dbg_list("mem: mem %p, addr %p, size %d\n",
                        mem, mem->addr, mem->size);

        }

        dbg_list("end of mem listing...\n");
}

static void free_mem(struct list_head *mem_list)
{
        struct list_head *tmp;
	struct rtls_mem *mem;
	struct list_head *head = mem_list;

        dbg_list("listing mem\n");
        list_for_each(tmp, head) {

		dbg_list("tmp = %p\n", tmp);
                mem = list_entry(tmp, struct rtls_mem, list);
		
                dbg_list("freeing mem: mem %p, addr %p, size %d\n",
                        mem, mem->addr, mem->size);
		
		list_del(head->next);
		free(mem->addr);
		
		//free(tmp);
        }

        dbg_list("end of mem listing...\n");
}

void rtls_free(void *mem)
{
	free(mem);
}

int __rtls_init(void)
{      
	dbg_list("init\n");	
        INIT_LIST_HEAD(&mem_list);
}

void __rtls_exit(void)
{       
	dbg_list("exit\n");	
	free_mem(&mem_list); 
	dbg_list("&mem_list = %p\n", &mem_list);
	print_list_mem(&mem_list);
}
