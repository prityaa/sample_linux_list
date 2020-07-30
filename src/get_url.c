#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <url.h>

#define DEBUG
#include <dbg.h>

#define SERVER_IP       "136.233.36.135"
#define SERVER_PORT     "8764"
#define OP_GEO_LOCATE   "geolocate"
#define OP_GEO_SUBMIT   "geosubmit"
#define API_VERSION     "3"
#define SERVICE_ID      "test-100"

void fill_url(struct struct_url *url)
{
        memset(url, '\0', sizeof(*url));

        strcpy(url->ip, SERVER_IP);
        strcpy(url->port, SERVER_PORT);
        strcpy(url->version, "v");
        strcat(url->version, API_VERSION);
        strcpy(url->operation, OP_GEO_LOCATE);
        //strcpy(url->service_id, service_ids[0]);
}

void get_url(char *url, const char *key, const char *operation)
{
        //fill_url(&s_url);

        memset(url, '\0', SIZE_256B);

        strcpy(url, "http://");
        strcat(url, SERVER_IP);
        strcat(url, ":");
        strcat(url, SERVER_PORT);
        strcat(url, "/");
        strcat(url, "v");
        strcat(url, API_VERSION);
        strcat(url, "/");
        strcat(url, operation);
        strcat(url, "?");
        strcat(url, "serviceid=");
        //strcat(url, "'MLS + Mozilla DB'");
        strcat(url, SERVICE_ID);
        strcat(url, "&apikey=");
        strcat(url, key);
        //dbg("url = %s\n", url);
}

