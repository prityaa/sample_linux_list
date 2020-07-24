#ifndef __DBG_H__
#define __DBG_H__

#ifdef DEBUG
#define debug	1	
#else
#define debug	0 						
#endif

#define dbg(fmt, ...) \
        do { if (debug) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__); } while (0)

#endif
