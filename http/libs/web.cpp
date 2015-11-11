#include <string>
#include <cstring>
#include <errno.h>
#include <algorithm>
#include <sys/types.h>
#include <event2/dns.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "web.hpp"
#include "../evhtp.h"
#include "logger.hpp"
#include "session.hpp"

void
zxy_add_http_header(
        zxy_request_context *context,
        const std::string& key,
        const std::string& val)
{
    const evhtp_request_t *request = context->request;
    evhtp_headers_add_header(
            request->headers_out,
            evhtp_header_new(key.c_str(), val.c_str(), 1, 1));
}

const std::string
zxy_get_http_header(
        zxy_request_context *context,
        const std::string& name
        )
{
    std::string result;
    evhtp_headers_t *headers = context->request->headers_in;
    const char *header_val = evhtp_header_find(headers, name.c_str());
    if ( header_val != NULL )
    {
        result = header_val;
    }
    return result;
}

bool zxy_get_http_header(
        zxy_request_context *context,
        const std::string& key,
        std::string& value
        )
{
    bool ret = false;
    evhtp_headers_t *headers = context->request->headers_in;
    const char *header_val = evhtp_header_find(headers, key.c_str());
    if ( header_val != NULL )
    {
        ret = true;
        value = header_val;
    }
    return ret;
}


const std::string
zxy_get_params(
        zxy_request_context *context,
        const std::string& name
        )
{
    std::string result;
    evhtp_query_t *query = context->request->uri->query;
    const char * val = evhtp_kv_find(query, name.c_str());
    if ( val != NULL )
    {
        result = val;
    }
    return result;
}

void
zxy_printf_content(
        zxy_request_context *context,
        const char* fmt,
        ...
        )
{
    evhtp_header_t *header = evhtp_headers_find_header(
            context->request->headers_out,
            "Content-Type"
            );
    if ( header == NULL )
    {
        zxy_add_http_header(context, "Content-Type", "text/html");
    }
    va_list args;
    va_start(args, fmt);
    evbuffer_add_vprintf(context->request->buffer_out, fmt, args);
    va_end(args);
}

void
zxy_send_content(
        zxy_request_context *context,
        const std::string& content
        )
{
    evhtp_header_t *header = evhtp_headers_find_header(
            context->request->headers_out,
            "Content-Type"
            );
    if ( header == NULL )
    {
        zxy_add_http_header(context, "Content-Type", "text/html");
    }
    evbuffer_add(context->request->buffer_out, content.c_str(), content.size());
}

#include "web.partial.sendfile.cpp"

void
zxy_redirect(
        zxy_request_context *context,
        const std::string& url
        )
{
    zxy_add_http_header(context, "Location", url);
    zxy_end_request(context, HTTP_SEEOTHER);
}

void
zxy_404(
        zxy_request_context *context,
        const std::string& msg
        )
{
    zxy_send_content(context, msg.c_str());
    zxy_end_request(context, HTTP_NOT_FOUND);
}

void
zxy_500(
        zxy_request_context *context,
        const std::string& msg
        )
{
    zxy_send_content(context, msg.c_str());
    zxy_end_request(context, HTTP_500);
}

static void
zxy_destroy_request_context(
    zxy_request_context* &context
    )
{
    delete context;
    context = NULL;
}

void
zxy_end_request(
        zxy_request_context *context,
        zxy_http_status status
        )
{
    evhtp_send_reply(context->request, status);
    session_end(context);
    zxy_destroy_request_context(context);
}

void
zxy_end_send_file(
        zxy_request_context *context
        )
{
    session_end(context);
    zxy_destroy_request_context(context);
}

bool
zxy_is_ajax(
        zxy_request_context *context
       )
{
    std::string isAjax = zxy_get_params(context, "isAjax");
    return !isAjax.empty();
    //std::string request_with = zxy_get_http_header(context, "HTTP_X_REQUESTED_WITH");
    //return request_with == "XMLHttpRequest";
}


#include "web.partial.web_request.cpp"

