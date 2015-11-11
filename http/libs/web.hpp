#ifndef LIB_WEB_INCLUDED
#define LIB_WEB_INCLUDED

#include <string>
#include <map>
#include <set>

#include <sqlite3.h>

#include "../evhtp.h"
#include "data_collect.h"


enum zxy_http_method {
    GET = htp_method_GET,
    POST = htp_method_POST
};

enum zxy_http_status {
    HTTP_OK = EVHTP_RES_OK,
    HTTP_PARTIAL = EVHTP_RES_PARTIAL,
    HTTP_SEEOTHER = EVHTP_RES_SEEOTHER,
    HTTP_500 = EVHTP_RES_500,
    HTTP_NOT_FOUND = EVHTP_RES_NOTFOUND
};

enum zxy_upstream_request_status {
    DNS_RESOLVE_ERR = 600,
    DNS_RESOLVE_UNKOWN_AIFAMILY = 601,
    REQ_ERR = 602,
    REQ_TIMEOUT = 603,
    REQ_OK = 666
};

enum zxy_user_platform {
    IPHONE = 1,
    IPOD = 2,
    IPAD = 4,
    IOS = 1 | 2 | 4,
    BLACKBERRY = 8,
    ANDROID = 9,
    WINPHONE = 10,
    PC = 11,
    MAC = 12,
    UNKNOWN_PLATFORM = 13
};

struct zxy_site_config {
    char *tpl_path;
    char *tpl_cache_path;
    char *db_path;
    char *logpath;
    char *logfile;
    int session_timeout;
    char *listen_address;
    int listen_port;
    int thread_count;
    int socket_backlog;
    char *user_host;
    char *apid;
    char *apid_path;
    char *unix_sock_filepath;
    int apid_len;
    int urltype_len;
    int uri_len;
    int mac_len;
    int useragent_len;
    int log_level;
    char* language_path;
    char* session_data;
    char* session_pos;
};
 
struct zxy_http_server {
    evbase_t *evbase;
    evhtp_t *htp;
    evdns_base *dnsbase;
    sqlite3* db;
    data_collect_info data;
    int arp_handle;
    int unix_sock;
    std::set<std::string> url_set;
    const zxy_site_config *site_config;
    std::string session_data;
};

struct zxy_request_context {
    std::string session_id;
    const zxy_http_server *http_server;
    evhtp_request_t *request;
    sqlite3 *db;
};

typedef void(*async_action_callback)(zxy_request_context*, void* arg);
typedef void(*zxy_upstream_request_handler)(
        zxy_request_context *context, 
        evhtp_request_t *upstream_req,
        const std::string& content, 
        const std::map<std::string, std::string>& cookies,
        zxy_upstream_request_status status, 
        void* lpParam);   // !!!!!!!!!!!!!!!!!!!!!

//if you return false
//you must end the request yourself. zxy_end_request
typedef bool(*filter_callback)(zxy_request_context*, void *arg);


void
zxy_add_http_header(
        zxy_request_context *context,
        const std::string& key,
        const std::string& val
        );


bool zxy_get_http_header(
        zxy_request_context *context,
        const std::string& key,
        std::string& value
        );

const std::string
zxy_get_http_header(
        zxy_request_context *context,
        const std::string& name
        );

const std::string
zxy_get_params(
        zxy_request_context *context,
        const std::string& name
        );

void 
zxy_printf_content(
        zxy_request_context *context,
        const char *fmt,
        ...
        );
void
zxy_send_content(
        zxy_request_context *context,
        const std::string& content
        );

void 
zxy_send_file(
        zxy_request_context *context,
        const std::string& file_path
        );

void
zxy_end_send_file(
        zxy_request_context *context
        );


void
zxy_redirect(
        zxy_request_context *context,
        const std::string& url
        );
void
zxy_404(
        zxy_request_context *context,
        const std::string& msg
       );

void
zxy_500(
        zxy_request_context *context,
        const std::string& msg
       );

void
zxy_end_request(
        zxy_request_context *context,
        zxy_http_status status
        );

void
zxy_upstream_request_uri(
        zxy_request_context *context,
        const std::string& uri,
        const std::map<std::string, std::string>& cookie,
        zxy_upstream_request_handler handler,
        void* lpParam = NULL
        );
bool
zxy_is_ajax(
        zxy_request_context *contex
       );

#endif
