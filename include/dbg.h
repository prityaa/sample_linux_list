#ifndef __DBG_H__
#define __DBG_H__

#ifdef DEBUG
#define debug	1	
#else
#define debug	0 						
#endif

#ifdef DEBUG_LIST
#define debug_list   1       
#else
#define debug_list   0                                               
#endif


#define dbg(fmt, ...) \
        do { if (debug) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__); } while (0);


#define dbg_list(fmt, ...) \
        do { if (debug_list) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__); } while (0);

#endif
