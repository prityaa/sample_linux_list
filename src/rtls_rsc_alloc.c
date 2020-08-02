#include <stdio.h>
#include <stdlib.h>
#include <rtls_rsc.h>
//#include <rtls_list.h>

#define DEBUG_LIST
#include <dbg.h>

/* head  of the rsc node */
static struct list_head mem_list;

void *rtls_malloc(int size)
{
	struct rtls_mem *mem;
	mem = (struct rtls_mem *)malloc(sizeof(*mem));
	void *temp = malloc(size);
	struct list_head *list = &mem->list;	

	mem->addr = temp;
	mem->size = size;
	
	dbg_list("temp = %p\n", temp);
	if (temp) {
		list_add(&(mem->list), &mem_list);
	}

	return temp;
}

static void print_rsc(struct list_head *mem_list)
{

        struct list_head *tmp;
	struct rtls_mem *mem;

        dbg_list("listing mem\n");
        list_for_each(tmp, mem_list) {
                mem = list_entry(tmp, struct rtls_mem, list);

                dbg_list("mem: addr %p, size %d\n",
                        mem->addr, mem->size);
        }

        dbg_list("end of mem listing...\n");
}

void rtls_free(void)
{
	//free();
}

int __rtls_init(void)
{      
	dbg_list("init\n");	
        INIT_LIST_HEAD(&mem_list);
}

void __rtls_exit(void)
{       
	dbg_list("exit\n");	
	print_rsc(&mem_list); 
}
