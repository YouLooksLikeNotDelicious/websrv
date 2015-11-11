#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <atomic>
#include <set>

#include "web.hpp"
#include "arp.h"
#include "data_collect.h"
#include "unix_socket.h"
#include "unix_socket_buffer.h"

static std::atomic<int> total_user(0);

static __thread struct msg *msg = NULL;
static __thread char data_info[4096];

void init_data_collector(struct zxy_http_server *server)
{
    server->url_set.insert("/account/forgetfirst");
    server->url_set.insert("/account/forgetsecond");
    server->url_set.insert("/account/forgetthird");
    server->url_set.insert("/main/index");
    server->url_set.insert("/search/index");
    server->url_set.insert("/search/result");
    server->url_set.insert("/search/game");
    server->url_set.insert("/search/video");
    server->url_set.insert("/search/music");
    server->url_set.insert("/search/wallpaper");
    server->url_set.insert("/search/soft");

    server->arp_handle = init_arp_table_handle();
    server->unix_sock = unix_socket_init(server->site_config->unix_sock_filepath);
}

void destroy_data_collector(struct zxy_http_server *server)
{
    close(server->unix_sock);
    close(server->arp_handle);
}


const char * get_user_agent(struct zxy_request_context *ctx)
{
    evhtp_headers_t *headers = ctx->request->headers_in;
    return evhtp_header_find(headers, "User-Agent");
}

const char * get_url(struct zxy_request_context *ctx)
{
    return ctx->request->uri->path->path;
}

int get_url_len(zxy_request_context *ctx)
{
    //return ctx->request->uri->path->match_end - ctx->request->uri->path->match_start;
    return strlen(ctx->request->uri->path->path);
}

char * get_mac_from_br_lan(int arp_handle, const char *ip, char *buf, int len)
{
    return get_mac(arp_handle, ip, "br_lan", buf, len);
}

char *get_apid(const char *filename, char *buf, int len)
{
    FILE *fp = fopen(filename, "r");
    int l = 0;
    if ( fp != NULL ) {
        char *ret = fgets(buf, len, fp);
        l = strlen(ret);
        if ( ret[l-1] == '\r' || ret[l-1] == '\n' ) {
            ret[l-1] = '\0';
        }
        if ( ret[l-2] == '\r' || ret[l-2] == '\n' ) {
            ret[l-2] = '\0';
        }
        fclose(fp);
        return ret;
    }
    return NULL;
}

bool check_page(zxy_request_context *ctx)
{
    const char *url = get_url(ctx);
    int url_len = get_url_len(ctx);
    if ( ctx->http_server->url_set.find(url) != ctx->http_server->url_set.end() ) {
        return true;
    }
    int cmp = strncmp(url + url_len - pagepost_uri_postfix.size(), pagepost_uri_postfix.c_str(), pagepost_uri_postfix.size());
    if ( cmp == 0 ) {
        return true;
    }
    return false;
}

bool check_preview_suffix(const char *url, int len)
{
    bool ret = true;
    int cmp = 0;
    cmp = strncmp(url + len - preview_uri_suffix1.size(), preview_uri_suffix1.c_str(), preview_uri_suffix1.size());
    if ( cmp != 0 ) {
        cmp = strncmp(url + len - preview_uri_suffix2.size(), preview_uri_suffix2.c_str(), preview_uri_suffix2.size());
        if ( cmp != 0 ) {
            ret = false;
        }
    }
    return ret;
}

bool check_preview(zxy_request_context *ctx)
{
    const char *        url         = get_url(ctx);
    int                 url_len     = get_url_len(ctx);
    bool                is_preview  = false;
    std::string         range;

    int cmp = strncmp(url, preview_uri_prefix.c_str(), preview_uri_prefix.size());
    if ( cmp == 0 ) {
        bool ret = false;
        ret = zxy_get_http_header(ctx, page_range_tag, range);
        if ( !ret  || (ret && range.substr(0, page_range_start.size()) == page_range_start) ) {
            is_preview = check_preview_suffix(url, url_len);
        }
    }

    return is_preview;
}

bool check_download(zxy_request_context *ctx)
{
    const char *        url         = get_url(ctx);
    std::string         range;
    bool                is_download = false;

    int cmp = strncmp(url, download_uri_prefix.c_str(), download_uri_prefix.size());
    if ( cmp == 0 ) {
        bool ret = false;
        ret = zxy_get_http_header(ctx, page_range_tag, range);
        if ( !ret || (ret && range.substr(0, page_range_start.size()) == page_range_start) ) {
            is_download = true;
        }
        else {
        }
    }
    return is_download;
}
bool check_connect(zxy_request_context *ctx) {
    bool ret = false;
    const char *url = get_url(ctx);
    int cmp = strncmp(url, connect_uri_prefix.c_str(), connect_uri_prefix.size());
    if ( cmp == 0 ) {
        ret = true;
        total_user += 1;
    }
    return ret;
}

bool check_disconnect(zxy_request_context *ctx)
{
    bool ret = false;
    const char *url = get_url(ctx);
    int cmp = strncmp(url, disconnect_uri_prefix.c_str(), disconnect_uri_prefix.size());
    if ( cmp == 0 ) {
        ret = true;
        total_user -= 1;
    }
    return ret;
}

char *get_ip(int fd, char *buf, int len)
{
    struct sockaddr_in addr;
    socklen_t sl = sizeof(struct sockaddr_in);
    int res = getpeername(fd, (struct sockaddr *)&addr, &sl);
    if ( res < 0 ) {
        return NULL;
    }
    strncpy(buf, inet_ntoa(addr.sin_addr), len);
    return buf; 
}

char *get_ip(struct zxy_request_context *ctx, char *buf, int len)
{
    int fd = ctx->request->conn->sock;
    struct sockaddr_in addr;
    socklen_t socklen = sizeof(struct sockaddr_in);
    int res = getpeername(fd, (struct sockaddr *)&addr, &socklen);
    if ( res < 0 ) {
        return NULL;
    }
    strncpy(buf, inet_ntoa(addr.sin_addr), len);
    return buf;
}

void init_data(struct data_collect_info *data, struct zxy_site_config *config)
{
    data->mac = (char *)malloc(config->mac_len);
    data->apid = (char *)malloc(config->apid_len);
    data->uri = (char *)malloc(config->uri_len);
    data->uritype = (char *)malloc(config->urltype_len);
    data->useragent = (char *)malloc(config->useragent_len);
    data->total = 0;

    const char *apid_file = config->apid_path;
    get_apid(apid_file, data->apid, config->apid_len);
}

void set_common_data(struct zxy_request_context *ctx, struct data_collect_info *data)
{
    char ip[32];
    get_ip(ctx, ip, sizeof(ip));
    get_mac_from_br_lan(ctx->http_server->arp_handle, ip, data->mac, ctx->http_server->site_config->mac_len);
    strncpy(data->uri, ctx->request->uri->path->full, ctx->http_server->site_config->uri_len);
    //strncpy(data->useragent, get_user_agent(ctx), ctx->http_server->site_config->useragent_len);
    const char* user_agent = get_user_agent(ctx);
    if ( user_agent != NULL )
    {
        strncpy(data->useragent, get_user_agent(ctx), ctx->http_server->site_config->useragent_len);
    }
    else
    {
        data->useragent[0] = '\0';
    }
}

void release_data(struct data_collect_info *data)
{
    free(data->mac);
    free(data->apid);
    free(data->uri);
    free(data->useragent);
    free(data->uritype);
}

bool data_collect(struct zxy_request_context *ctx, struct data_collect_info *data)
{
    bool status = false;
    set_common_data(ctx, data);
    int n;
    int unix_sock = ctx->http_server->unix_sock;
    
    if ( msg == NULL ) {
        msg = init_msg();
    }

    int urltype_len = ctx->http_server->site_config->urltype_len;
    status = check_page(ctx);
    if ( status ) {
        strncpy(data->uritype, "page", urltype_len);
        goto need_send;
    }

    status = check_connect(ctx);
    if ( status ) {
        goto need_send;
    }

    status = check_disconnect(ctx);
    if ( status ) {
        goto need_send;
    }

    status = check_download(ctx);
    if ( status ) {
        strncpy(data->uritype, "download", urltype_len);
        goto need_send;
    }

    status = check_preview(ctx);
    if ( status ) {
        strncpy(data->uritype, "preview", urltype_len);
        goto need_send;
    }
    goto end;
    data->total = total_user;
    if ( unix_sock < 0 ) {
        goto end;
    }

need_send:
    n = snprintf(data_info, sizeof(data_info), "longdata|%s|%s|%s|%s|%s|%s|||||", data->uritype, data->uri, data->mac, data->apid, "uid", data->useragent);
    append_msg(msg, data_info, n);
    unix_socket_sendmsg(unix_sock, msg);

end:
    return true;
}
