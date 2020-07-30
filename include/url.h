#ifndef __URL_H__
#define __URL_H__

#define SIZE_8B		8
#define SIZE_16B	16
#define SIZE_256B	256

#if 0
const char *service_ids[] = {
	"test-100:", 	/* MLS + Mozilla DB */
	"test-101:", 	/* MLS + Jio DB	 */
	"test-102:", 	/* Combain */
	"test-103:", 	/* Skyhook */
	"test-104:", 	/* Trilateration */
	"test-200:", 	/* All */
};
#endif

struct struct_url {
	char ip[SIZE_16B];
	char operation[SIZE_16B];
	char port[SIZE_8B];
	char service_id[SIZE_8B];	
	char version[SIZE_8B];
};

void get_url(char *url, const char *key);
#endif

