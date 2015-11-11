#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <event2/dns.h>

#include <confuse.h>

#include "evhtp.h"
#include "controller.hpp"
#include "libs/logger.hpp"
#include "libs/web.hpp"
#include "libs/session.hpp"
#include "libs/mimetype.hpp"
#include "libs/daemon.h"
#include "libs/cmdopt.h"
#include "libs/arp.h"
#include "libs/data_collect.h"
#include "libs/filter_login_auth.hpp"
#include "libs/unix_socket.h"
#include "controller/SiteLanguage.hpp"
#include "controller/Business/SessionBusiness.hpp"

typedef void (*zxy_http_request_handler)(zxy_request_context* context);

struct zxy_evhtp_callback_params
{
    zxy_http_server *server;
    zxy_http_request_handler callback;
};

typedef std::vector<zxy_evhtp_callback_params*> callback_params_type;
static callback_params_type callback_params;
typedef std::vector<std::pair<filter_callback, void *> > filter_list_type;
static filter_list_type before_filter_list;

static bool
zxy_check_url(zxy_request_context *context, void *arg)
{
    bool ret = true;
    arg = NULL;

    char * path = context->request->uri->path->path;
    char *pos = strstr(path, "..");

    if ( pos != NULL ) {
        ret = false;
    }

    if ( ret ) {
        pos = strchr(path, '\'');
        if ( pos != NULL ) {
            ret = false;
        }
    }

    if ( ret ) {
        pos = strchr(path, '\\');
        if ( pos != NULL ) {
            ret = false;
        }
    }

    if ( !ret ) {
        logerr("[Warnning], someone trying to inject our http server");
        zxy_end_request(context, HTTP_500);
    }
    return ret;
}

static void
zxy_create_request_context(
    zxy_request_context *context,
    zxy_http_server* server,
    evhtp_request_t* req
    )
{
    context->http_server  = server;
    context->request = req;
    context->db = server->db;
}

static void
zxy_register_before_process_filter(
        filter_callback filter,
        void *arg
        )
{
    before_filter_list.push_back(std::pair<filter_callback, void*>(filter, arg));
}


// default request handler 
static void
zxy_default_request_handler(zxy_request_context *context)
{
    std::string file_path = ".";
    file_path += context->request->uri->path->full;
    //log(file_path.c_str());
    if ( !zxy_check_url(context, NULL) ) {
        return;
    }
    zxy_send_file(context, file_path);
    //zxy_send_content(context,  std::string("This is a default page! \nThe page you find is not exists"));
    //zxy_end_request(context, HTTP_OK);
}

static void
zxy_evhtp_callback(evhtp_request_t *req, void *arg)
{
    log("Handle %s", req->uri->path->full);
    zxy_evhtp_callback_params *callback_params = (zxy_evhtp_callback_params*)arg;
    zxy_http_request_handler callback = callback_params->callback;

    //在end_request的时候对象被销毁
    zxy_request_context *context = new zxy_request_context();
    zxy_create_request_context(context, callback_params->server, req);

    session_start(context);

    //data_collect(context, &callback_params->server->data);

    if ( callback != zxy_default_request_handler )
    {
        int i = 0;
        int filter_size = before_filter_list.size();
        for ( i = 0; i < filter_size; i++ )
        {
            std::pair<filter_callback, void*> filter = before_filter_list[i];
            if ( !filter.first(context, filter.second) )
            {
                return;
            }
        }
    }

    const char *connection_header = evhtp_header_find(req->headers_in, "Connection");

    if ( connection_header == NULL )
    {
        zxy_add_http_header(context, "Connection", "close");
    }
    else
    {
        zxy_add_http_header(context, "Connection", connection_header);
    }

    zxy_add_http_header(context, "X-Power-By", "LONGDATA/libevhtp");
    callback(context);
}

static evhtp_callback_t *
zxy_register_request_handler(
    zxy_http_server *server,
    const char *url,
    zxy_http_request_handler cb
    )
{
    zxy_evhtp_callback_params *params = new zxy_evhtp_callback_params();
    params->server = server;
    params->callback = cb;

    callback_params.push_back(params);
    evhtp_callback_t * cb_t = NULL;
    if ( url == NULL )
    {
        log("register a default process handler");
        evhtp_set_gencb(server->htp, zxy_evhtp_callback, params);
    }
    else 
    {
        log("register %s", url);
        cb_t = evhtp_set_cb(server->htp, url, zxy_evhtp_callback, params);

        if ( cb_t == NULL )
        {
            logerr("Register url route error");
            abort();
        }
    }
    return cb_t;
}

static void
zxy_destroy_callback_params()
{
    int callback_size = callback_params.size();
    for ( int i = 0; i < callback_size; i++ )
    {
        delete callback_params[i];
    }
    callback_params.clear();
}

static void
zxy_init_http_server(
        zxy_http_server* http_server,
        zxy_site_config* config
    )

{
    sqlite3 *db = NULL;
    int status = sqlite3_open(config->db_path, &db);
    if ( status == SQLITE_OK )
    {
        http_server->db = db;
    }
    else
    {
        //const char *err_msg = sqlite3_errstr(status);
        //logerr(err_msg);
        logerr("open sqlite3 database error");
        abort();
    }

    evbase_t *evbase = event_base_new();
    evhtp_t *htp = evhtp_new(evbase, NULL);
    evdns_base *dnsbase = evdns_base_new(evbase, 0);
    evdns_base_nameserver_ip_add(dnsbase, "127.0.0.1");
    //evdns_base_nameserver_ip_add(dnsbase, "223.5.5.5");
    //evdns_base_nameserver_ip_add(dnsbase, "223.6.6.6");
    //evdns_base_nameserver_ip_add(dnsbase, "8.8.8.8");

    // zxy_register_before_process_filter((filter_callback)data_collect, &http_server->data);
    zxy_register_before_process_filter((filter_callback)filter_login_auth, NULL);

    http_server->evbase = evbase;
    http_server->dnsbase = dnsbase;
    http_server->htp = htp;
    http_server->site_config = config;
    http_server->session_data = config->session_data;

    init_data_collector(http_server);
    init_data(&http_server->data, config);
    config->apid = http_server->data.apid;

    // g_theSiteLanguage.InitializeMap(config->language_path);

    http_server->unix_sock = unix_socket_init(config->unix_sock_filepath);
    http_server->arp_handle = init_arp_table_handle();

    if ( config->thread_count > 1 )
    {
        evhtp_use_threads(htp, NULL, config->thread_count, NULL);
    }

    evhtp_bind_socket(htp, config->listen_address, config->listen_port, config->socket_backlog);
    
    init_log(config->logpath, config->logfile, config->log_level);
    g_theSiteLanguage.InitializeMap(config->language_path);
    std::string csSessionPos = config->session_pos;
    std::string csSessionData = config->session_data;
    g_theSessionBusiness.InitSession(csSessionPos, csSessionData);

    log("server is listening on %s:%d", config->listen_address, config->listen_port);
}

static void
zxy_destroy_http_server(
    zxy_http_server* http_server
    )
{
    evhtp_unbind_socket(http_server->htp);

    sqlite3_close(http_server->db);
    release_data(&http_server->data);

    evhtp_free(http_server->htp);
    evdns_base_free(http_server->dnsbase, 1);
    event_base_free(http_server->evbase);

    http_server->htp = NULL;
    http_server->evbase = NULL;

    destroy_data_collector(http_server);
    release_log();
}


static void
zxy_http_server_main_loop(
    zxy_http_server* http_server
    )
{
    event_base_loop(http_server->evbase, 0);
}


static void
zxy_register_router(
    zxy_http_server* http_server
    )
{
    zxy_register_request_handler(http_server, "/Account/loginPage", LoginPage);
    zxy_register_request_handler(http_server, "/Account/rechargePage", RechargePage);
    zxy_register_request_handler(http_server, "/Account/recharge", Recharge);

    zxy_register_request_handler(http_server, NULL, zxy_default_request_handler);
}

static void
zxy_release_site_config(
        zxy_site_config *config
        )
{
    free(config->tpl_path);
    free(config->tpl_cache_path);
    free(config->db_path);
    free(config->listen_address);
    free(config->user_host);
    free(config->apid_path);
    free(config->unix_sock_filepath);
}

static void
zxy_init_site_config(
    zxy_site_config* config,
    const std::string &configfile
    )
{
    cfg_opt_t opts[] =
    {
        CFG_STR((char *)"tpl_path", (char *)"/web/webserver_wifistore/tpl", CFGF_NONE),
        CFG_STR((char *)"tpl_cache_path", (char *)"/tmp/tpl", CFGF_NONE),
        CFG_STR((char *)"apid_path", (char *)"/tmp/APID", CFGF_NONE),
        CFG_STR((char *)"unix_sock_path", (char *)"/tmp/data_collecter.sock", CFGF_NONE),
        CFG_STR((char *)"db_path", (char *)"data.db", CFGF_NONE),
        CFG_STR((char *)"listen_address", (char *)"127.0.0.1", CFGF_NONE),

        CFG_STR((char *)"logpath", (char *)"/mnt/sda1/log", CFGF_NONE),
        CFG_STR((char *)"logfile", (char *)"longdata_evhtp.log", CFGF_NONE),
        CFG_INT((char *)"session_timeout", 1800, CFGF_NONE),
        CFG_INT((char *)"listen_port", 8080, CFGF_NONE),
        CFG_INT((char *)"thread_count", 1, CFGF_NONE),
        CFG_INT((char *)"socket_backlog", 1024, CFGF_NONE),
        CFG_STR((char *)"user_host", (char*)"", CFGF_NONE),
        CFG_INT((char *)"mac_len", 32, CFGF_NONE),
        CFG_INT((char *)"apid_len", 64, CFGF_NONE),
        CFG_INT((char *)"uri_len", 1024, CFGF_NONE),
        CFG_INT((char *)"urltype_len", 16, CFGF_NONE),
        CFG_INT((char *)"useragent_len", 128, CFGF_NONE),
        CFG_INT((char *)"log_level", 0, CFGF_NONE),
        CFG_STR((char *)"language_path", (char*)"assets/language_package/english.package", CFGF_NONE),
        CFG_STR((char *)"session_data",(char*)"session_data.ini", CFGF_NONE),
        CFG_STR((char *)"session_pos", (char*)"session_pos.ini", CFGF_NONE),
        CFG_END()
    };
    cfg_t *cfg;
    cfg = cfg_init(opts, CFGF_NONE);

    if(cfg_parse(cfg, configfile.c_str()) == CFG_PARSE_ERROR)
        abort();
    char *tpl_path = cfg_getstr(cfg, "tpl_path");
    char *tpl_cache_path = cfg_getstr(cfg, "tpl_cache_path");
    char *db_path = cfg_getstr(cfg, "db_path");
    int session_timeout = cfg_getint(cfg, "session_timeout");
    char *listen_address = cfg_getstr(cfg, "listen_address");
    int listen_port = cfg_getint(cfg, "listen_port");
    int thread_count = cfg_getint(cfg, "thread_count");
    int socket_backlog = cfg_getint(cfg, "socket_backlog");
    char *user_host = cfg_getstr(cfg, "user_host");
    char *language_path = cfg_getstr(cfg, "language_path");
    char *session_data = cfg_getstr(cfg, "session_data");
    char *session_pos = cfg_getstr(cfg, "session_pos");

    config->tpl_path = strdup(tpl_path);
    config->tpl_cache_path = strdup(tpl_cache_path);
    config->db_path = strdup(db_path);
    config->session_timeout = session_timeout;
    config->listen_address = strdup(listen_address);
    config->listen_port = listen_port;
    config->thread_count = thread_count;
    config->socket_backlog = socket_backlog;
    config->user_host = strdup(user_host);
    config->unix_sock_filepath = strdup(cfg_getstr(cfg, "unix_sock_path"));
    config->apid_path = strdup(cfg_getstr(cfg, "apid_path"));
    config->apid_len = cfg_getint(cfg, "apid_len");
    config->mac_len = cfg_getint(cfg, "mac_len");
    config->uri_len = cfg_getint(cfg, "uri_len");
    config->urltype_len = cfg_getint(cfg, "urltype_len");
    config->useragent_len = cfg_getint(cfg, "useragent_len");
    config->log_level = cfg_getint(cfg, "log_level");
    config->logpath = strdup(cfg_getstr(cfg, "logpath"));
    config->logfile = strdup(cfg_getstr(cfg, "logfile"));
    config->language_path = strdup(language_path);
    config->session_data = strdup(session_data);
    config->session_pos = strdup(session_pos);

    cfg_free(cfg);
}

int main(int argc, char **argv)
{
    option op;
    char *daemon_buf = NULL;

    sleep(2);

    parser_cmd(&op, argc, argv);

    zxy_site_config config;
    zxy_init_site_config(&config, op.config);

    if ( op.daemon ) {
        daemon_buf = make_daemon_init();
    }

    zxy_http_server server;
    zxy_init_http_server(&server, &config);

    zxy_register_router(&server);

    session_init();
    init_mimetype_map();

    zxy_http_server_main_loop(&server);

    session_destroy_all();
    destroy_mimetype_map();
    zxy_destroy_http_server(&server);
    zxy_destroy_callback_params();

    if ( daemon_buf != NULL ) {
        make_daemon_release(daemon_buf);
    }

    zxy_release_site_config(&config);

    return 0;
}

