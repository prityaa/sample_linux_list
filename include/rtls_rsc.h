#ifndef __RTLS_RSC_H__
#define __RTLS_RSC_H__
#include <rtls_list.h>

struct rtls_mem {
	struct list_head list;	
	void *addr;
	int size;
};

struct rtls_rsc {
	/* add more rsc */
	struct rtls_mem mem;
	//struct config config;
};

int __rtls_init(void);
void __rtls_exit(void);
void *rtls_malloc(int size);
void rtls_free(void);
#endif
