#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define DEBUG
#include <dbg.h>

#include <rtlsRestApi.h>
#include <httPostReqResp.h>

#include <json.hpp>
using json = nlohmann::json;

/* url to test site */
const char *url = "http://136.233.36.135:8764/v3/getapikey";

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
	std::cout << "rtlsGetApiKey json body " << 
		jsonObj.dump(4) << std::endl;
	std::string post_data = jsonObj.dump();

	ret = httpPostRequest(url, post_data, &rsp_code, &rsp_data);
	dbg("Response code = %ld, date = %p\n", 
			rsp_code, rsp_data.payload);
	if((rsp_code == 200) && (rsp_data.payload != NULL)) {
		/* parse return */
		jsonRsp = json::parse(rsp_data.payload);
#ifdef DEBUG	
		std::cout << jsonRsp.dump(4) << std::endl;
		std::cout << "Response code from json:" 
			<< jsonRsp["code"] << std::endl;
		std::cout << "apikey from json:" 
			<< jsonRsp["apikey"] << std::endl;
#endif
		std::string api_key_str = jsonRsp["apikey"].get<std::string>();
		strcpy(respOut->apiKey, api_key_str.c_str());
		free(rsp_data.payload);
	} else {
		dbg("falied to recieve api Key\n");
		return -1;
	}
	return ret;
}
