#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

//#define DEBUG
#include <dbg.h>

#include <rtlsRestApi.h>
#include <httPostReqResp.h>

#include <json.hpp>
using json = nlohmann::json;

/* url to test site */
const char *url = "http://136.233.36.135:8764/v3/getapikey";

void fill_api_key_resp(json jsonRsp, struct getApiKeyResp_t *resp)
{

	memset(resp, '\0', sizeof(*resp));
#ifdef DEBUG
	/* print api key response */
	std::cout << "Api Key Response =>\n" << std::endl;
	std::cout << jsonRsp.dump(4) << std::endl;
#endif

#ifdef DEBUG
	std::cout << "apikey (unassigned):"
		<< jsonRsp["apikey"] << std::endl;
#endif
	std::string temp_str = jsonRsp["apikey"].get<std::string>();
	strcpy(resp->apiKey, temp_str.c_str());
	dbg("apikey = %s\n", resp->apiKey);

	/* fill name */
#ifdef DEBUG
	std::cout << "name (unassigned):"
		<< jsonRsp["userId"]["name"] << std::endl;
#endif
	temp_str = jsonRsp["userId"]["name"].get<std::string>();
	strcpy(resp->uid.name, temp_str.c_str());
	dbg("name = %s\n", resp->uid.name);
	/* fill email */
#ifdef DEBUG
	std::cout << "email (unassigned):"
		<< jsonRsp["userId"]["email"] << std::endl;
#endif
#if 0
	temp_str = jsonRsp["userId"]["email"].get<std::string>();
	if (temp_str.c_str()) {
		strcpy(resp->uid.email, temp_str.c_str());
		dbg("email = %s\n", resp->uid.email);	
	}

	/* fill imei */
#ifdef DEBUG
	std::cout << "imei (unassigned):"
		<< jsonRsp["userId"]["imei"] << std::endl;
#endif
	temp_str = jsonRsp["userId"]["imei"].get<std::string>();
	strcpy(resp->uid.imei, temp_str.c_str());
	dbg("imei = %s\n", resp->uid.email);
#endif

	/* fill imei */
#ifdef DEBUG
	std::cout << "time (unassigned):"
		<< jsonRsp["time"] << std::endl;
#endif
	temp_str = jsonRsp["time"].get<std::string>();
	strcpy(resp->time, temp_str.c_str());
	dbg("time = %s\n", resp->time);

	/* fill statusCode */
#ifdef DEBUG
	std::cout << "respCode (unassigned):"
		<< jsonRsp["code"] << std::endl;
#endif
	resp->respCode = jsonRsp["code"].get<int>();

	dbg("respCode = %d\n", resp->respCode);
}

int rtlsGetApiKey(struct getApiKeyReq_t *reqParams, 
		getApiKeyResp_t *respOut) {
	int ret = 0;
	json jsonRsp;
	long rsp_code;
	struct respData_t rsp_data;

	//dbg("getApiKey Enter\n");

	/* create json object body for post */  
	json j, jsonObj;
	j["name"] = reqParams->uid.name;
	j["email"] = reqParams->uid.email;
	j["imei"] = reqParams->uid.imei;
	jsonObj["userId"] = j;
#ifdef DEBUG	
	std::cout << "rtlsGetApiKey json body " << 
		jsonObj.dump(4) << std::endl;
#endif
	std::string post_data = jsonObj.dump();

	ret = httpPostRequest(url, post_data, &rsp_code, &rsp_data);
	dbg("Response code = %ld, date = %p\n", 
			rsp_code, rsp_data.payload);
	if((rsp_code == 200) && (rsp_data.payload != NULL)) {
		/* parse return */
		jsonRsp = json::parse(rsp_data.payload);
		
		fill_api_key_resp(jsonRsp, respOut);
		free(rsp_data.payload);
	} else {
		dbg("falied to recieve api Key\n");
		return -1;
	}
	return ret;
}
