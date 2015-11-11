#include <stdio.h>
#include <vector>

#include "web.hpp"
#include "session.hpp"

static std::vector<std::string> ignore_urls;
class init_urls {
    public:
    init_urls() { 
        ignore_urls.push_back("/Account/loginPage");
    };
};

static init_urls obj;

bool filter_login_auth(zxy_request_context *context, void *arg)
{
    int i = 0;
    int len = ignore_urls.size();
    char *url = context->request->uri->path->full;
    for ( i = 0; i < len; i++ )
    {
        if ( ignore_urls[i] == url )
        {
            return true;
        }
    }

    int* tmp = (int*)session_get(context, "isLogin");
    if (tmp == NULL || (*tmp) != 1)
    {
        if ( zxy_is_ajax(context) )
        {
            zxy_send_content(context, "{\"status\": 438}");
            zxy_end_request(context, HTTP_OK);
        }
        else
        {
            zxy_redirect(context, "/Account/loginPage");
        }
        return false;
    }
    return true;
}
