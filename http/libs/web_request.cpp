#include <string>
#include <cstring>
#include <algorithm>

#include "web_request.hpp"
#include "logger.hpp"


static void
copy_context(
    const struct zxy_http_connection_context *context,
    struct zxy_http_connection_context *new_context
    )
{
    new_context->session_id = new char[std::strlen(context->session_id) + 1];
    std::strcpy(new_context->session_id, context->session_id);
    new_context->site_config = context->site_config;
    new_context->connection = context->connection;
    new_context->http_server = context->http_server;
}

bool request_get(
   struct zxy_http_connection_context *context,
   const std::string& request_uri,
   std::string cookie,
   request_callback callback
   )
{
    struct mg_server *server = context->http_server->real_server;
    
    Uri uri = Uri::Parse(request_uri);

    request_option *option = new request_option();
    struct mg_connection *client = mg_connect(server, uri.Host.c_str(), uri.Port, 0);
    if ( client == NULL )
    {
        return false;
    }
    else
    {
        client->connection_param = option;

        copy_context(context, &(option->context));
        option->callback = callback;
        option->host = uri.Host;
        option->protocol = uri.Protocol;
        option->path = uri.Path;
        option->query = uri.QueryString;
        option->method = "GET";
        option->cookie = cookie;
        return true;
    }
}

