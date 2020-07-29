#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <rtlsRestApi.h>

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


int main(int argc, char *argv[]) 
{
	int ret = 0;

	/* packets to get api key */
	struct getApiKeyReq_t key_req;
	struct getApiKeyResp_t key_resp;

	/* packet to get geo ordinates */
	struct geoLocReq_t gloc_req;
	struct geoLocResp_t gloc_resp;

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
	//dbg("key = %s\n", api_key);
	printf("\n\n++++++++++++++++");
	printf(" GEO LOCATE ");
	printf("++++++++++++++++\n\n");
#endif	
#if 1
	/* get geo locate */
	/**
	 * FEEDME :
	 * struct req to be filled by MTK/kaios to get geo location
	 *
	 * example explained with fill_geo_locate_req_packets
	 */
	fill_geo_locate_req_packets(&gloc_req, key_resp.apiKey);

	ret = rtlsGeoLocate(&gloc_req, &gloc_resp);
	dbg("ret = %d\n", ret);
	if (!ret) {
		printf("failed to get Geo location\n");	
		return -2;
	}
#endif
	return 0;
}
