#ifndef _HTTP_POST_REQ_RESP_H_
#define _HTTP_POST_REQ_RESP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>

/* Http request response data */
struct respData_t {
    char *payload;
    size_t size;
};

int httpPostRequest(const std::string&, std::string, long*, struct respData_t*); 

#endif //_HTTP_POST_REQ_RESP_H_
