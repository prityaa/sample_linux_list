#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <rtlsRestApi.h>

int main(int argc, char *argv[]) {
    char apiKey[256] = {0};
    int ret = 0;
    struct getApiKeyReq_t req = {0};
    struct getApiKeyResp_t resp = {0};

    strcpy(req.uid.name, "testuser");
    req.uid.email[0] = '\0';
    req.uid.imei[0] = '\0';

    printf("RTLS REST API implemenation\n");
    
    ret = rtlsGetApiKey(req, &resp);
    
    if(ret == 0) {
        printf("Got get Api key, api key = %s\n", resp.apiKey);
    } else {
        printf("Failed to get Api key, error code %d\n", ret);
    }
    
    return 0;
}
