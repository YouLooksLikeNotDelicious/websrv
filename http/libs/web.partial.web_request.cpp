#include <string>
#include <cstring>
#include <algorithm>

#include "../libs/web.hpp"
#include "../libs/logger.hpp"
#include "../evhtp.h"

//--- upstream request module --//
struct Uri
{
public:
    std::string QueryString, Path, Protocol, Host;
    int Port;
    
    Uri() : Port(80) {}

    static Uri *Parse(const std::string &uri)
    {
        Uri *result = new Uri();
 
        typedef std::string::const_iterator iterator_t;

        if (uri.length() == 0)
            return result;

        iterator_t uriEnd = uri.end();

        // get query start
        iterator_t queryStart = std::find(uri.begin(), uriEnd, '?');

        // protocol
        iterator_t protocolStart = uri.begin();
        iterator_t protocolEnd = std::find(protocolStart, uriEnd, ':');            //"://");

        if (protocolEnd != uriEnd)
        {
            std::string prot = &*(protocolEnd);
            if ((prot.length() > 3) && (prot.substr(0, 3) == "://"))
            {
                result->Protocol = std::string(protocolStart, protocolEnd);
                protocolEnd += 3;   //      ://
            }
            else
                protocolEnd = uri.begin();  // no protocol
        }
        else
            protocolEnd = uri.begin();  // no protocol

        // host
        iterator_t hostStart = protocolEnd;
        iterator_t pathStart = std::find(hostStart, uriEnd, '/');  // get pathStart

        iterator_t hostEnd = std::find(protocolEnd, 
            (pathStart != uriEnd) ? pathStart : queryStart,
            ':');  // check for port

        result->Host = std::string(hostStart, hostEnd);

        // port
        if ((hostEnd != uriEnd) && ((&*(hostEnd))[0] == ':'))  // we have a port
        {
            hostEnd++;
            iterator_t portEnd = (pathStart != uriEnd) ? pathStart : queryStart;
            //result->Port = std::atoi(std::string(hostEnd, portEnd).c_str());
            result->Port = atoi(std::string(hostEnd, portEnd).c_str());
        }

        // path
        if (pathStart != uriEnd)
            result->Path = std::string(pathStart, uri.end());

        if (result->Path.empty()) 
        {
            result->Path = "/";
        }

        // query
        if (queryStart != uriEnd)
            result->QueryString = std::string(queryStart, uri.end());

        return result;

    }   // Parse
};

struct upstream_request_callback_params {
    zxy_request_context *context;
    zxy_upstream_request_handler handler;
    Uri *uri;
    std::string cookie;
    void *lpParam;
};

static int
get_cookie_it_cb(
        evhtp_header_t *header,
        void *arg
        )
{
    std::map<std::string, std::string> *cookies = (std::map<std::string, std::string>*)arg;
    if ( std::strcmp(header->key, "Set-Cookie") == 0 )
    {
        char *header_val = header->val;

        const char *tmp1 = strchr(header_val, '=');
        int len = tmp1 - header_val;
        char cookie_name[len + 1];
        cookie_name[len] = 0;
        strncpy(cookie_name, header_val, len);

        const char *tmp2 = strchr(header_val, ';');
        if ( tmp2 == NULL )
        {
            len = strlen(header_val) - 1 - len;
        }
        else
        {
            len = tmp2 - tmp1 - 1;
        }
        char cookie_val[len + 1];
        cookie_val[len] = 0;
        strncpy(cookie_val, tmp1 + 1, len);

        (*cookies)[cookie_name] = cookie_val;
    }

    return 0;
}

static void 
upstream_request_callback(
        evhtp_request_t *upstream_req, 
        void *arg
        )
{
    log("Request finish. length: %ld", evbuffer_get_length(upstream_req->buffer_in));
    upstream_request_callback_params *params = (upstream_request_callback_params*)arg;
    zxy_request_context *context = params->context;
    zxy_upstream_request_handler handler = params->handler;
    void* lpParam = params->lpParam;
    delete params->uri;
    delete params;

    std::string content;

    int len = evbuffer_get_length(upstream_req->buffer_in);
    char content_c[len + 1];
    content_c[len] = '\0';
    evbuffer_copyout(upstream_req->buffer_in, content_c, len);

    content = content_c;
    log("## content: %s", content_c);
    
    std::map<std::string, std::string> cookies;
    evhtp_headers_for_each(upstream_req->headers_in, get_cookie_it_cb, &cookies);

    handler(context, upstream_req, content, cookies, REQ_OK, lpParam);
}

static void
upstream_request_error_cb(
        evhtp_request_t *upstream_req,
        evhtp_error_flags errtype,
        void *arg)
{
    //0x20 BEV_EVENT_ERROR 0x40 BEV_EVENT_TIMEOUT
    if ( (0x20 & errtype) == 0x20 || (0x40 & errtype) == 0x40 )
    {
        log("Request error happens flags: %d", errtype);
        upstream_request_callback_params *params = (upstream_request_callback_params*)arg;
        zxy_upstream_request_handler handler = params->handler;
        std::map<std::string, std::string> cookies;

        zxy_upstream_request_status status = REQ_ERR;
        if ((0x40 & errtype) == 0x40) {
            status = REQ_TIMEOUT;
        }
        handler(params->context, upstream_req, "", cookies, status, params->lpParam);
        delete params->uri;
        delete params;
    }
}


static void
evdns_query_callback(
        int errcode,
        evutil_addrinfo *res,
        void *arg
        )
{
    upstream_request_callback_params *params;
    params = (upstream_request_callback_params*)arg;
    char host_ip[128];
    if ( errcode )
    {
        std::map<std::string, std::string> cookies;
        log("DNS resolve %s error: %s", params->uri->Host.c_str(), evutil_gai_strerror(errcode));
        params->handler(params->context, NULL,  "", cookies, DNS_RESOLVE_ERR, params->lpParam);
        delete params->uri;
        delete params;
        evutil_freeaddrinfo(res);
        return;
    }
    else
    {
        if ( res->ai_family == AF_INET )
        {
            sockaddr_in *sin = (sockaddr_in*)res->ai_addr;
            evutil_inet_ntop(AF_INET, &sin->sin_addr, host_ip, 128);
            log("DNS resolve. ipv4 %s", host_ip);
        }
        else if ( res->ai_family == AF_INET6 )
        {
            sockaddr_in6 *sin6 = (sockaddr_in6 *)res->ai_addr;
            evutil_inet_ntop(AF_INET6, &sin6->sin6_addr, host_ip, 128);
            log("DNS resolve. ipv6 %s", host_ip);
        }
        else
        {
            log("DNS resolve. Can not know the address info family");
            std::map<std::string, std::string> cookies;
            params->handler(params->context, NULL, "", cookies, DNS_RESOLVE_UNKOWN_AIFAMILY, params->lpParam);
            delete params->uri;
            delete params;
            evutil_freeaddrinfo(res);
            return;
        }
    }

    evutil_freeaddrinfo(res);

    evbase_t *evbase = params->context->http_server->evbase;
    Uri *uri = params->uri;

    evhtp_connection_t *conn = evhtp_connection_new(evbase, host_ip, uri->Port);
    if (conn == NULL || conn->error == 1) {
        std::map<std::string, std::string> cookies;
        params->handler(params->context, NULL, "", cookies, REQ_ERR, params->lpParam);
        delete params->uri;
        delete params;
        return;
    }
    timeval read_timeval = {40, 0}; // 40 sec for read
    timeval write_timeval = {40, 0}; // 40 sec for write
    //timeval read_timeval = {2, 0}; // 40 sec for read
    //timeval write_timeval = {2, 0}; // 40 sec for write
    evhtp_connection_set_timeouts(conn, &read_timeval, &write_timeval);

    evhtp_request_t *upstream_req = evhtp_request_new(upstream_request_callback, params);

    //handle error
    evhtp_set_hook(&upstream_req->hooks, evhtp_hook_on_error, (evhtp_hook)upstream_request_error_cb, params);

    std::string cookie = params->cookie;
    evhtp_headers_add_header(
            upstream_req->headers_out, 
            evhtp_header_new("Host", uri->Host.c_str(), 0, 0));
    evhtp_headers_add_header(
            upstream_req->headers_out, 
            evhtp_header_new("Connection", "close", 0, 0));
    evhtp_headers_add_header(
            upstream_req->headers_out, 
            evhtp_header_new("Cookie", cookie.c_str(), 0, 0));
            
    std::string request_path = uri->Path/* + uri->QueryString*/;
    log("Request %s", request_path.c_str());
    evhtp_make_request(conn, upstream_req, htp_method_GET, request_path.c_str());
}

std::string zxy_cookiemap_to_string(const std::map<std::string, std::string>& cookiemap)
{
    auto it = cookiemap.begin();

    std::string cookie;

    char szBuff[0x100] = {0};

    for (; it != cookiemap.end(); ++it)
    {
        std::string key = (*it).first;

        std::string value = (*it).second;

        snprintf(szBuff, sizeof(szBuff), "%s=%s;", key.c_str(), value.c_str());

        cookie += szBuff;
    }

    return cookie;
}

void
zxy_upstream_request_uri(
        zxy_request_context *context,
        const std::string& request_uri,
        const std::map<std::string, std::string>& cookie,
        zxy_upstream_request_handler handler,
        void *lpParam
        )
{
    Uri *uri = Uri::Parse(request_uri);

    upstream_request_callback_params *params = new upstream_request_callback_params();
    params->context = context;
    params->handler = handler;
    params->uri = uri;
    params->cookie = zxy_cookiemap_to_string(cookie);
    params->lpParam = lpParam;

    evutil_addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = EVUTIL_AI_CANONNAME;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    log("Resolve domain name %s", uri->Host.c_str());

    evdns_getaddrinfo(
            context->http_server->dnsbase, uri->Host.c_str(), NULL ,
            //context->http_server->dnsbase, "54.217.204.56", NULL ,
            &hints, evdns_query_callback, params
            );
}

//-- end upstream request module --//
