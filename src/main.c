#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <rtlsRestApi.h>

#define DEBUG
#include <dbg.h>

void fill_key_req_packats(struct getApiKeyReq_t *req)
{
	memset(req, '\0', sizeof(*req));
	strcpy(req->uid.name, "testuser");
}

void fill_lte_cell(struct lteCell_t *lteCells)
{
#if 1
        lteCells->mcc = 404;
        lteCells->mnc = 45;
        lteCells->tac = 9001;
        lteCells->cellId = 409090;
        lteCells->rssi = -20;

        (lteCells+1)->mcc = 405;
        (lteCells+1)->mnc = 861;
        (lteCells+1)->tac = 22;
        (lteCells+1)->cellId = 409090;
        (lteCells+1)->rssi = -98;

        (lteCells+2)->mcc = 405;
        (lteCells+2)->mnc = 861;
        (lteCells+2)->tac = 139;
        (lteCells+2)->cellId = 34316048;
        (lteCells+2)->rssi = -82;
#endif
}

void fill_geo_locate_req_packets(struct geoLocReq_t *req,
		const char *key)
{
	struct lteCell_t *lteCells;
	int i = 0;

	memset(req, '\0', sizeof(*req));

	strcpy(req->apiKey, key);

	/* fill lte cell */
	req->numLteCells = 3;
	lteCells = (struct lteCell_t *)
		malloc((req->numLteCells)*sizeof(*lteCells));
	memset(lteCells, '\0', (req->numLteCells)*sizeof(struct lteCell_t));

#if 0        
	lteCells->mcc = 404;
	lteCells->mnc = 45;
	lteCells->tac = 9001;
	lteCells->cellId = 409090;
	lteCells->rssi = -20;

	(lteCells+1)->mcc = 405;
	(lteCells+1)->mnc = 861;
	(lteCells+1)->tac = 22;
	(lteCells+1)->cellId = 409090;
	(lteCells+1)->rssi = -98;

	(lteCells+2)->mcc = 405;
	(lteCells+2)->mnc = 861;
	(lteCells+2)->tac = 139;
	(lteCells+2)->cellId = 34316048;
	(lteCells+2)->rssi = -82;
#endif
	fill_lte_cell(lteCells);
	req->lteCells = lteCells;
#if 0
	for (i = 0; i < req->numLteCells; i++) {
		printf("CellId = %d %d\n", i, (req->lteCells[i].cellId));
		printf("mcc = %d %d\n", i, (req->lteCells[i].mcc));
		printf("mnc = %d %d\n", i, (req->lteCells[i].mnc));
		printf("rssi = %d %d\n", i, (req->lteCells[i].rssi));
		printf("tac = %d %d\n", i, (req->lteCells[i].tac));
	}
#endif
}

void fill_geo_submit_req_packets(struct geoSubReq_t *gsub_req, 
		struct getApiKeyResp_t *key_resp,
		struct geoLocResp_t *gloc_resp)
{
	struct geoSubItem_t *item;
	strcpy(gsub_req->apiKey, key_resp->apiKey);
	
#if 0	
	/* FIXME
	 * as of now, hardcoding values, 
	 * need to take vaues from geoLocResp_t 
	 */

	item = (struct geoSubItem_t *)malloc(sizeof(struct geoSubItem_t));
	item->position.lat = 12.974518;
	item->position.lng = 77.67275;
	item->position.accuracy = 1000.0;
	item->position.age = 5000; 

	gsub_req->items = item;
#endif
}

void print_all_data(struct getApiKeyResp_t *key_resp, 
		struct geoLocResp_t *gloc_resp, 
		struct geoSubResp_t *gsub_resp)
{
	printf("\n\n-----------------");
	printf(" prit all data ");
	printf("-----------------\n");

	printf("\n\nAPI_key =>\n");
	printf("name : %s\n", key_resp->uid.name);
	printf("email : %s\n", key_resp->uid.email);
	printf("imei : %s\n", key_resp->uid.imei);
	printf("time : %s\n", key_resp->time);
	printf("respCode : %d\n", key_resp->respCode);
	printf("key : %s\n", key_resp->apiKey);

	printf("\n\nGeo Locate =>\n");
	printf("statusMessage = %s\n", gloc_resp->statusMessage);
	printf("startTime = %s\n", gloc_resp->startTime);
	printf("endTime = %s\n", gloc_resp->endTime);
	printf("provider = %s\n", gloc_resp->locations->provider);
	printf("statusCode = %d\n",
                        gloc_resp->locations->statusCode);
	printf("statusMsg = %s\n", gloc_resp->locations->statusMsg);
	printf("latitude = %f\n", gloc_resp->locations->latitude);
	printf("longitude = %f\n", gloc_resp->locations->longitude);
	printf("accuracy = %f\n", gloc_resp->locations->accuracy);
	printf("code = %d\n", gloc_resp->locations->code);
	printf("time = %s\n", gloc_resp->locations->time);

	printf("\n\nGeo Submit =>\n");
	printf("time = %s\n", gsub_resp->time);
	printf("resp->code = %d\n", gsub_resp->code);
	printf("message = %s\n", gsub_resp->message);
}


int main(int argc, char *argv[]) 
{
	int ret = 0;

	/* packets to get api key */
	struct getApiKeyReq_t key_req;
	struct getApiKeyResp_t key_resp;

	/* packet to get geo ordinates */
	struct geoLocReq_t gloc_req;
	struct geoLocResp_t gloc_resp;

	/* packet to geo submit */
	struct geoSubReq_t gsub_req;
	struct geoSubResp_t gsub_resp;

	/**
	 * FEEDME :
	 * struct req to be filled by MTK/kaios to get api key
	 * 
	 * example explained with fill_key_req_packats
	 */
	fill_key_req_packats(&key_req);

	/* request key from RTLS server */
	ret = rtlsGetApiKey(&key_req, &key_resp);
	if (!ret) {
		printf("\napi key = %s\n", key_resp.apiKey);
	} else {
		printf("Failed to get Api key\n");
		return -1;
	}

#ifdef DEBUG
	/* get key */	
	printf("\n\n++++++++++++++++");
	printf(" GEO LOCATE ");
	printf("++++++++++++++++\n\n");
#endif	

	/* get geo locate */
	/**
	 * FEEDME :
	 * struct req to be filled by MTK/kaios to get geo location
	 *
	 * example explained with fill_geo_locate_req_packets
	 */
	fill_geo_locate_req_packets(&gloc_req, key_resp.apiKey);
	
	/* fill provider */
        gloc_resp.locations = (struct respLocation_t *)malloc
                (sizeof(struct respLocation_t));
	
	ret = rtlsGeoLocate(&gloc_req, &gloc_resp);
	dbg("ret = %d\n", ret);
	if (ret) {
		printf("failed to get Geo location\n");
		return -2;
	}

#ifdef DEBUG
	/* get key */	
	printf("\n\n++++++++++++++++");
	printf(" GEO SUBMIT ");
	printf("++++++++++++++++\n\n");
#endif	

	/* geo submit */
        /**
         * FEEDME :
         * struct req to be filled by MTK/kaios to get geo location
         *
         * example explained with fill_geo_submit_req_packets
         */

	fill_geo_submit_req_packets(&gsub_req, &key_resp, &gloc_resp);

	ret = rtlsGeoSubmit(&gsub_req, &gsub_resp);
	dbg("ret = %d\n", ret);
	if (ret) {
		printf("failed to submit\n");
		return -3;
	}

	/* print all data */
	print_all_data(&key_resp, &gloc_resp, &gsub_resp);

	return 0;
}
