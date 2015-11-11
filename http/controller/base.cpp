#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <cmath>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../libs/web.hpp"
#include "base.hpp"
#include "../libs/logger.hpp"
#include "../libs/arp.h"

#include "SiteLanguage.hpp"

static char * 
get_tpl_path(
    zxy_request_context *context,
    const char *tpl_name
    )
{
    char *tpl_path = context->http_server->site_config->tpl_path;
    char * tpl_full_path = (char *)malloc(strlen(tpl_path) + strlen(tpl_name) + 1);

    if (tpl_full_path != NULL)
    {
        strcpy(tpl_full_path, tpl_path);
        strcat(tpl_full_path, tpl_name);

        return tpl_full_path;
    }

    return NULL;
}

static void
free_tpl_path(
    char *tpl_full_path
    )
{
    free(tpl_full_path);
}

static char *
get_tpl_cache_path(
    struct zxy_request_context *context,
    const char *tpl_name
    )
{
    char *tpl_cache_path = context->http_server->site_config->tpl_cache_path;
    char * tpl_full_path = (char *)malloc(strlen(tpl_cache_path) + strlen(tpl_name) + 1);

    if (tpl_full_path != NULL)
    {
        strcpy(tpl_full_path, tpl_cache_path);
        strcat(tpl_full_path, tpl_name);

        return tpl_full_path;
    }

    return NULL;
}

static void
free_tpl_cache_path(
    char *tpl_cache_full_path
    )
{
    free(tpl_cache_full_path);
}


static char * 
read_file(
    const char *file_path
    )
{
    struct stat file_stat;
    stat(file_path, &file_stat);

    off_t file_size = file_stat.st_size;
    int file_d;
    if ( (file_d = open(file_path, O_RDONLY)) == -1)
    {
        abort();
    }

    char *file_content = (char *)malloc(file_size + 1);

    ssize_t read_size;
    if ( (read_size = read(file_d, file_content, file_size)) == -1)
    {
        abort();
    }
    file_content[file_size] = 0; // add null-terminate end

    close(file_d);
    return file_content;
}

static void
free_file_content (
    char *file_content
    )
{
    free(file_content);
}

char * 
read_tpl(
    struct zxy_request_context *context,
    const char *tpl_name
    )
{
    char *tpl_cache_path = get_tpl_cache_path(context, tpl_name);
    char *tpl_content;
    if ( access(tpl_cache_path, F_OK) != -1 )
    {
        tpl_content = read_file(tpl_cache_path);
    }
    else
    {
        char *tpl_path = get_tpl_path(context, tpl_name);
        tpl_content = read_file(tpl_path);
        free_tpl_path(tpl_path);
    }
    free_tpl_cache_path(tpl_cache_path);
    return tpl_content;

}

void
free_tpl(
    char *tpl_content
    )
{
    free_file_content(tpl_content);
}

int
return_content(
        zxy_request_context *context,
        const std::string& content
        )
{
    zxy_send_content(context, content);
    zxy_end_request(context, HTTP_OK);
    return 0;
}

int return_json(
        zxy_request_context *context,
        const std::string& content
        )
{
    zxy_add_http_header(context, "Content-Type", "application/json");
    zxy_send_content(context, content);
    zxy_end_request(context, HTTP_OK);
    return 0;
}

int return_jsonp(
        zxy_request_context *context,
        const std::string& content
        )
{
    std::string callback = zxy_get_params(context, "callback");
    if ( callback.empty() )
    {
        zxy_500(context, "No callback from frontend");
        return -1;
    }
    zxy_add_http_header(context, "Content-Type", "text/javascript");
    std::string result = callback + "(\"" + content + "\")"; 
    zxy_send_content(context, result);
    zxy_end_request(context, HTTP_OK);
    return 0;
}

int return_file(
        zxy_request_context *context,
        const std::string& file_path
        )
{
    zxy_send_file(context, file_path);
    return 0;
}

int return_redirect(
        zxy_request_context *context,
        const std::string& url
        )
{
    zxy_redirect(context, url);
    return 0;
}

extern zxy_user_platform 
get_user_platform(
        zxy_request_context *context
        )
{
    std::string user_agent = zxy_get_http_header(context, "User-Agent");
    if ( user_agent.empty() )
    {
        return UNKNOWN_PLATFORM;
    }
    if ( user_agent.find("Android") != std::string::npos )
    {
        return ANDROID;
    }
    else if ( user_agent.find("iPad") != std::string::npos )
    {
        return IPAD;
    }
    else if ( user_agent.find("iPhone") != std::string::npos )
    {
        return IPHONE;
    }
    else if ( user_agent.find("iPod") != std::string::npos )
    {
        return IPOD;
    }
    else if ( user_agent.find("Windows Phone") != std::string::npos )
    {
        return WINPHONE;
    }
    else if ( user_agent.find("BlackBerry") != std::string::npos )
    {
        return BLACKBERRY;
    }
    else if ( user_agent.find("Windows") != std::string::npos )
    {
        return PC;
    }
    else if ( user_agent.find("Macintosh") != std::string::npos )
    {
        return MAC;
    }
    return UNKNOWN_PLATFORM;
}

extern std::string
ucfirst(
        const std::string& str
       )
{
    std::string result;
    result = std::toupper(str[0]);
    result += str.substr(1);
    return result;
}

extern std::string
htmlentities(
        const std::string& str,
        quotestyle q_style,
        const std::string& char_set
        )
{
    return str;
}

extern std::string
get_params_from_query(
        zxy_request_context *context,
        const std::string& name
        )
{
    return zxy_get_params(context, name);
}

extern std::string
get_params_from_post(
        zxy_request_context *context,
        const std::string& name
        )
{
    abort();
    logerr("Not implement");
    return NULL;
}

extern bool
is_ios(
        zxy_request_context *context
      )
{
    zxy_user_platform platform = get_user_platform(context);
    return (platform & IOS) == platform;
}

extern std::string
formatsizeunits(
        float size_bytes
        )
{
    char result[32];
    if ( size_bytes >= 1073741824 )
    {
        snprintf(result, 32, "%.2f GB", size_bytes / 1073741824);
    }
    else if ( size_bytes >= 1048576 )
    {
        snprintf(result, 32, "%.2f MB", size_bytes / 1048576);
    }
    else if ( size_bytes > 1024 )
    {
        snprintf(result, 32, "%.2f KB", size_bytes / 1024);
    }
    else
    {
        snprintf(result, 32, "%.2f B", size_bytes);
    }
    return result;
}


extern std::string
formatduration(
        float time_sec
        )
{
    std::string result;
    int hours = std::floor(time_sec / 3600);
    time_sec -= hours*3600;
    int minutes = std::floor(time_sec / 60);
    time_sec -= minutes * 60;
    if ( hours > 0 )
    {
        result += std::to_string(hours) + " hours ";
    }
    if ( minutes > 0 )
    {
        result += std::to_string(minutes) + " min ";
    }
    if ( time_sec > 0 )
    {
        result += std::to_string((int)std::ceil(time_sec)) + " sec";
    }
    if ( result.empty() )
    {
        result = "0 sec";
    }
    return result;
}

extern std::string
basename(
        const std::string& file_path
        )
{
    size_t pos = file_path.rfind('/');
    if ( pos != std::string::npos )
    {
        return file_path.substr(pos + 1);
    }
    return std::string();
}

extern std::string
strtolower(
        const std::string& str
        )
{
    int len = str.size();
    char result[len + 1];
    result[len] = 0;
    std::transform(str.begin(), str.end(), result, ::tolower);
    return result;
}
extern std::string
GetMac(zxy_request_context* ctx)
{
    char mac[32] = {0};
    char ip[32] = {0};

    evhtp_request_t* req = ctx->request;

    int handle = init_arp_table_handle();

    inet_ntop(AF_INET, &((struct sockaddr_in*)req->conn->saddr)->sin_addr, ip, sizeof(ip));
    std::string realIp = zxy_get_http_header(ctx, "X-Real-Ip");

    get_mac(handle, realIp.c_str(), "br-ra", mac, sizeof(mac));

    release_arp_table_handle(handle);

    return mac;

}
namespace std {

extern std::string
to_string(
        const std::string& str
        )
{
    return str;
}

}


extern bool 
init_language_package(const std::string& language_path)
{
    // return true;
    return g_theSiteLanguage.InitializeMap(language_path);
}

extern std::string
L(
        const std::string& key
        )
{
    // return "";
    return g_theSiteLanguage.get_value_by_key(key);
}
