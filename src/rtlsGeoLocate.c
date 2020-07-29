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
//using json = nlohmann::json;
using json = nlohmann::basic_json<std::map, std::vector, std::string, bool, std::int64_t, std::uint64_t, float>;
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


void fill_gloc_resp(json jsonRsp, struct geoLocResp_t *resp)
{
	/* fill statusMessage */
#ifdef DEBUG
	//std::cout << jsonRsp.dump(4) << std::endl;
	std::cout << "statusMessage (unassigned):"
		<< jsonRsp["status"]["statusMessage"] << std::endl;
#endif
	std::string temp_str = jsonRsp["status"]["statusMessage"].get<std::string>();
	strcpy(resp->statusMessage, temp_str.c_str());
	dbg("statusMessage = %s\n", resp->statusMessage);

	/* fill startTime */
#ifdef DEBUG
	//std::cout << jsonRsp.dump(4) << std::endl;
	std::cout << "startTime (unassigned):"
		<< jsonRsp["status"]["startTime"] << std::endl;
#endif
	temp_str = jsonRsp["status"]["startTime"].get<std::string>();
	strcpy(resp->startTime, temp_str.c_str());
	dbg("startTime = %s\n", resp->startTime);

	/* fill endTime */
#ifdef DEBUG
	//std::cout << jsonRsp.dump(4) << std::endl;
	std::cout << "endTime (unassigned):"
		<< jsonRsp["status"]["endTime"] << std::endl;
#endif
	temp_str = jsonRsp["status"]["endTime"].get<std::string>();
	strcpy(resp->endTime, temp_str.c_str());
	dbg("endTime = %s\n", resp->endTime);
	
	/* fill provider */
	resp->locations = (struct respLocation_t *)malloc
		(sizeof(struct respLocation_t));

#ifdef DEBUG
        std::cout << "provider (unassigned):"
                << jsonRsp["locations"][0]["provider"] << std::endl;
#endif
	/** TODO */
	//temp_str = jsonRsp["locations"][0]["provider"].get<std::to_string>();
	//strcpy(resp->locations->provider, temp_str.c_str());
	//dbg("provider = %s\n", temp_str);

	/* fill statusCode */
#ifdef DEBUG
        std::cout << "code (unassigned):"
                << jsonRsp["locations"][0]["status"]["code"] << std::endl;
#endif
	resp->locations->statusCode = jsonRsp["locations"][0]["status"]["code"].get<int>();

	dbg("resp->locations->statusCode = %d\n", 
			resp->locations->statusCode);
	std::cout << jsonRsp.dump(4) << std::endl;

	/* fill statusMsg */
#ifdef DEBUG
	std::cout << "statusMsg (unassigned):"
		<< jsonRsp["locations"][0]["status"]["message"] << std::endl;
#endif
	temp_str = jsonRsp["locations"][0]["status"]["message"].get<std::string>();
	strcpy(resp->locations->statusMsg, temp_str.c_str());
	dbg("statusMsg = %s\n", resp->locations->statusMsg);

        /* fill latitude */
#ifdef DEBUG
        std::cout << "latitude (unassigned):"
                << jsonRsp["locations"][0]["lat"] << std::endl;
#endif
        resp->locations->latitude = jsonRsp["locations"][0]["lat"].get<float>();
        dbg("latitude = %f\n", resp->locations->latitude);


        /* fill longitude */
#ifdef DEBUG
        std::cout << "longitude (unassigned):"
                << jsonRsp["locations"][0]["lng"] << std::endl;
#endif
        resp->locations->longitude = jsonRsp["locations"][0]["lng"].get<float>();
        dbg("longitude = %f\n", resp->locations->longitude);

	/* fill accuracy */
#ifdef DEBUG
        std::cout << "accuracy (unassigned):"
                << jsonRsp["locations"][0]["accuracy"] << std::endl;
#endif
	resp->locations->accuracy = jsonRsp["locations"][0]["accuracy"].get<float>();
	dbg("accuracy = %f\n", resp->locations->accuracy);

	/* fill code */
#ifdef DEBUG
        std::cout << "code (unassigned):"
                << jsonRsp["code"] << std::endl;
#endif
        resp->locations->code = jsonRsp["code"].get<int>();
        dbg("code = %d\n", resp->locations->code);

	/* fill time */
#ifdef DEBUG
        std::cout << "statusMsg (unassigned):"
                << jsonRsp["time"] << std::endl;
#endif
        temp_str = jsonRsp["time"].get<std::string>();
        strcpy(resp->locations->time, temp_str.c_str());
        dbg("time = %s\n", resp->locations->time);

}

int rtls_geo_locate(struct geoLocReq_t *req, 
		struct geoLocResp_t *resp)
{
	int ret = 0, i = 0;
	json jsonRsp;
	long rsp_code;
	struct respData_t rsp_data;
	char url[SIZE_256B];

	/* create json object body for geo locate post */
	json j, jsonObj;

#if 1
	for (; i < req->numLteCells; i++) {
                j["mcc"] = req->lteCells[i].mcc;
                j["mnc"] = req->lteCells[i].mnc;
                j["tac"] = req->lteCells[i].tac;
                j["cellId"] = req->lteCells[i].cellId;
                j["rssi"] = req->lteCells[i].rssi;
                jsonObj["ltecells"][i] = j;
	}	
#else  /* REMOVEME once all finalize */
		j["mcc"] = 404;
		j["mnc"] = 45;
		j["tac"] = 9001;
		j["cellId"] = 409090;
		j["rssi"] = -20;	  
		jsonObj["ltecells"][0] = j;

		j["mcc"] = 405;
		j["mnc"] = 861;
		j["tac"] = 22;
		j["cellId"] = 409090;
		j["rssi"] = -98;
		jsonObj["ltecells"][1] = j;

		j["mcc"] = 405;
		j["mnc"] = 861;
		j["tac"] = 139;
		j["cellId"] = 34316048;
		j["rssi"] = -82;
		jsonObj["ltecells"][2] = j;
#endif
	std::string post_data = jsonObj.dump();
#ifdef DEBUG
	std::cout << "geolocate json body\n" << jsonObj.dump(4) << std::endl;
#endif
	get_url(url, req->apiKey);
	dbg("url = %s\n", url);

	ret = httpPostRequest(url, post_data, &rsp_code, &rsp_data);
	dbg("Response code = %ld, ret = %d\n", rsp_code, ret);

#if 1
	if((rsp_code == 200) && (rsp_data.payload != NULL) && !ret) {
		/* parse return */
		jsonRsp = json::parse(rsp_data.payload);
	
#if 0
		jsonRsp = json::parseFloat(rsp_data.payload);
                std::cout << jsonRsp.dump(4) << std::endl;
                std::cout << "Response code from json:"
                        << jsonRsp["code"] << std::endl;
                std::cout << "apikey from json:"
                        << jsonRsp["status"]["statusMessage"] << std::endl;
#endif	
		fill_gloc_resp(jsonRsp, resp);	
		free(rsp_data.payload);
	} else {
		dbg("Err in getting location\n");
		return -2;
	}
#endif	
	return 0;	
}

int rtlsGeoLocate(struct geoLocReq_t *gloc_req,
                struct geoLocResp_t *gloc_resp)	
{
        return rtls_geo_locate(gloc_req, gloc_resp);
}

