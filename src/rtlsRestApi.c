#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "rtlsRestApi.h"
#include "httPostReqResp.h"

#include "json.hpp"
using json = nlohmann::json;

/* url to test site */
const char *url = "http://136.233.36.135:8764/v3/getapikey";

int rtlsGetApiKey(struct getApiKeyReq_t reqParams, getApiKeyResp_t *respOut) {
    int ret = 0;
    json jsonRsp;

    printf("getApiKey Enter\n");
  
    /* create json object body for post */  
    json j, jsonObj;
    j["name"] = reqParams.uid.name;
    j["email"] = reqParams.uid.email;
    j["imei"] = reqParams.uid.imei;
    jsonObj["userId"] = j;
    std::cout << "rtlsGetApiKey json body " << jsonObj.dump(4) << std::endl;
    std::string post_data = jsonObj.dump();
    
    long rsp_code;
    struct respData_t rsp_data;
    
    ret = httpPostRequest(url, post_data, &rsp_code, &rsp_data);

    printf("Response code = %ld\n", rsp_code);
    
    if((rsp_code == 200) && (rsp_data.payload != NULL)) {
        printf("Response data = %s\n", rsp_data.payload);
    
        /* parse return */
        jsonRsp = json::parse(rsp_data.payload);

        std::cout << jsonRsp.dump(4) << std::endl;
        std::cout << "Response code from json:" << jsonRsp["code"] << std::endl;
        std::cout << "apikey from json:" << jsonRsp["apikey"] << std::endl;
        
        std::string api_key_str = jsonRsp["apikey"].get<std::string>();
        strcpy(respOut->apiKey, api_key_str.c_str());
        
        free(rsp_data.payload);
    }
    return 0;
}

int getGeoLocation(struct LocCoords_t *loc){
    printf("getGeoLocation Enter\n");
    return 0;
}

int putGeoLocation(struct LocCoords_t *loc) {
    printf("putGeoLocation Enter\n");
    return 0;
}

