#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define DEBUG
#include <dbg.h>
#include <url.h>

#include <rtlsRestApi.h>
#include <httPostReqResp.h>

#include <json.hpp>
using json = nlohmann::json;

#define SERVER_IP	"136.233.36.135"
#define SERVER_PORT 	"8764"
#define	OP_GEO_LOCATE 	"geolocate"
#define	API_VERSION 	"3"
#define	SERVICE_ID	"test-100"

void fill_url(struct struct_url *url)
{
	memset(url, '\0', sizeof(*url));

	strcpy(url->ip, SERVER_IP);
	strcpy(url->port, SERVER_PORT);
	strcpy(url->version, "v");
	strcat(url->version, API_VERSION);
	strcpy(url->operation, OP_GEO_LOCATE);
	strcpy(url->service_id, service_ids[0]);
}

void get_url(char *url, const char *key)
{
	//dbg("kkey = %s\n", key);
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
	strcat(url, OP_GEO_LOCATE);
	strcat(url, "?");
	strcat(url, "serviceid=");
	//strcat(url, "'MLS + Mozilla DB'");
	strcat(url, SERVICE_ID);
	strcat(url, "&apikey=");
	strcat(url, key);
	//dbg("url = %s\n", url);
}

void fill_lte_cell(struct lteCell_t *lteCells)
{
	lteCells->mcc = 401;
	lteCells->mnc = 402;
	lteCells->tac = 9001;
	lteCells->cellId = 409090;
	lteCells->rssi = -20;
}

void fill_geo_locate_packets(struct geoLocReq_t *req,  
                struct geoLocResp_t *resp, const char *key) 
{ 
	struct lteCell_t lteCells;

        memset(req, '\0', sizeof(*req)); 
        memset(&lteCells, '\0', sizeof(struct lteCell_t)); 
        strcpy(req->apiKey, key); 
        strcpy(req->serviceId, SERVICE_ID);

	/* fill lte cell */
	fill_lte_cell(&lteCells);
	req->lteCells = &lteCells;		

        memset(resp, '\0', sizeof(*resp)); 
} 

int rtls_geo_locate(struct geoLocReq_t *req, 
		struct geoLocResp_t *resp, const char *key)
{
	int ret = 0;
	json jsonRsp;
	long rsp_code;
	struct respData_t rsp_data;
	char url[SIZE_256B];

	/* create json object body for geo locate post */
	json j, jsonObj;

	j["mcc"] = 401;
	j["mnc"] = 402;
	j["tac"] = 9001;
	j["cellId"] = 409090;
	j["rssi"] = -20;	  
	jsonObj["ltecells"][0] = j;
	std::string post_data = jsonObj.dump();

	std::cout << "geolocate json body\n" << jsonObj.dump(4) << std::endl;

	get_url(url, key);

	dbg("url = %s\n", url);

	ret = httpPostRequest(url, post_data, &rsp_code, &rsp_data);
	dbg("Response code = %ld, ret = %d\n", rsp_code, ret);

#if 1
	if((rsp_code == 400) && (rsp_data.payload != NULL) && !ret) {
		/* parse return */
		jsonRsp = json::parse(rsp_data.payload);

		free(rsp_data.payload);
	} else {
		dbg("Err in getting location\n");
		return -2;
	}
#endif	
	return 0;	
}

int get_geo_locate(const char *key)
{
        struct geoLocReq_t req;
        struct geoLocResp_t resp;

        fill_geo_locate_packets(&req, &resp, key);
        rtls_geo_locate(&req, &resp, key);
}

