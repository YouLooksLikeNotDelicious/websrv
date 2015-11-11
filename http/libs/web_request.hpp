#ifndef WEB_REQUEST_INCLUDED
#define WEB_REQUEST_INCLUDED
#include <string>
#include "../typedef.hpp"

typedef int (*request_callback)(struct zxy_http_connection_context *context, 
        const char *content, 
        int content_len,
        int status_code);

class request_option
{
public:
    struct zxy_http_connection_context context;
    request_callback callback;
    std::string host;
    std::string protocol;
    std::string path;
    std::string query;
    std::string method;
    std::string cookie;
    int port;
};

bool 
request_get(
    struct zxy_http_connection_context *context,
    const std::string& uri,
    const std::string cookie,
    request_callback callback
    );
#endif
