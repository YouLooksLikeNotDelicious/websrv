#ifndef __FILTER_H__
#define __FILTER_H__

#include <string>

#include "web.hpp"

struct data_collect_info {
    char *mac;
    char *apid;
    char *uri;
    char *uritype;
    char *useragent;
    int total;
};

static const std::string download_uri_prefix =  "/download";
static const std::string preview_uri_prefix = "/upload";
static const std::string preview_uri_suffix1 = ".mp4";
static const std::string preview_uri_suffix2 = ".mp3";
static const std::string disconnect_uri_prefix = "/disconnect";
static const std::string pagepost_uri_postfix = "page";
static const std::string page_range_start = "bytes=0";
static const std::string connect_uri_prefix = "/connect";

static const std::string page_range_tag = "Range";

void init_data_collector(struct zxy_http_server *server);
void release_data(struct data_collect_info *data);
void destroy_data_collector(struct zxy_http_server *server);
void init_data(struct data_collect_info *data, struct zxy_site_config *config);
bool data_collect(struct zxy_request_context *ctx, struct data_collect_info *data);

#endif
