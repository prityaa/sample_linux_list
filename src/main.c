#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <rtlsRestApi.h>

#define DEBUG
#include <dbg.h>

int get_geo_locate(const char *key);

void fill_req_resp_packats(struct getApiKeyReq_t *req, 
		struct getApiKeyResp_t *resp)
{
	memset(req, '\0', sizeof(*req));
	strcpy(req->uid.name, "testuser");
	
	memset(resp, '\0', sizeof(*resp));
}

int get_api_key(char *api_key)
{
	struct getApiKeyReq_t req;
	struct getApiKeyResp_t resp;;

	fill_req_resp_packats(&req, &resp);

	/* request key from RTLS server */
	if (!rtlsGetApiKey(req, &resp)) {
		printf("\napi key = %s\n", resp.apiKey);
		strcpy(api_key, resp.apiKey);
	} else {
		printf("Failed to get Api key, error code\n");
		return -1;
	}
	
	return 0;
}

int main(int argc, char *argv[]) 
{
	char api_key[256] = {0};
	int ret = 0;

	/* get key */	
	if (get_api_key(api_key))
		return -1;

	//dbg("key = %s\n", api_key);
	printf("\n\n++++++++++++++++");
	printf(" GEO LOCATE ");
	printf("++++++++++++++++\n\n");
	/* get geo locate */
	if (get_geo_locate(api_key))
		return -2;

	return 0;
}
