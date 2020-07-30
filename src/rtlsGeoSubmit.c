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

void fill_gsub_resp(json jsonRsp, struct geoSubResp_t *resp)
{
	std::string temp_str;

	memset(resp, '\0', sizeof(*resp));
#ifdef DEBUG
	/* print Geo Submit response */
	std::cout << "Geo Submit Response =>\n" << std::endl;
	std::cout << jsonRsp.dump(4) << std::endl;
#endif

	/* fill time */
#ifdef DEBUG
        std::cout << "time (unassigned):"
                << jsonRsp["time"] << std::endl;
#endif
	temp_str = jsonRsp["time"].get<std::string>();
        strcpy(resp->time, temp_str.c_str());
        dbg("time = %s\n", resp->time);

	/* fill statusCode */
#ifdef DEBUG
        std::cout << "code (unassigned):"
                << jsonRsp["code"] << std::endl;
#endif
        resp->code = jsonRsp["code"].get<std::float_round_style>();
        dbg("resp->code = %d\n", resp->code);

	/* fill message */
#ifdef DEBUG
        std::cout << "message (unassigned):"
                << jsonRsp["message"] << std::endl;
#endif
	temp_str = jsonRsp["message"].get<std::string>();
        strcpy(resp->message, temp_str.c_str());
        dbg("message = %s\n", resp->message);
}

int rtls_geo_submit(struct geoSubReq_t *req, 
		struct geoSubResp_t *resp)
{
	int ret = 0, i = 0;
	json jsonRsp;
	long rsp_code;
	struct respData_t rsp_data;
	char url[SIZE_256B];
	json j, j_item, jsonObj;

	/* create json object body for geo locate post */
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

	j_item["ltd"] = 12.974518;
	j_item["lng"] = 77.67275;
	j_item["accuracy"] = 1000.0;
	j_item["age"] = 5000;
	jsonObj["position"][0] = j_item;

	std::string post_data = jsonObj.dump();
#ifdef DEBUG
	std::cout << "geosubmit json body\n" << jsonObj.dump(4) << std::endl;
#endif
	get_url(url, req->apiKey, "geosubmit");
	dbg("url = %s\n", url);

	ret = httpPostRequest(url, post_data, &rsp_code, &rsp_data);
	dbg("Response code = %ld, ret = %d\n", rsp_code, ret);

	if((rsp_code == 200) && (rsp_data.payload != NULL) && !ret) {
		/* parse return */
		jsonRsp = json::parse(rsp_data.payload);
	
		fill_gsub_resp(jsonRsp, resp);	
		free(rsp_data.payload);
	} else {
		dbg("Err in geo submit\n");
		return -3;
	}
	return 0;	
}

int rtlsGeoSubmit(struct geoSubReq_t *gsub_req,
                struct geoSubResp_t *gsub_resp)	
{
        return rtls_geo_submit(gsub_req, gsub_resp);
}

