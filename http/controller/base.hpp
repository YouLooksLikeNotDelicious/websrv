
#ifndef CONTROLLER_COMMON_INCLUDED
#define CONTROLLER_COMMON_INCLUDED

#include "../libs/web.hpp"

#define echo(format,...) \
    zxy_send_content(context, format, ## __VA_ARGS__)

#define GAME "game"
#define VIDEO "video"
#define MUSIC "music"
#define PICTURE "picture"
#define SOFT "soft"

#define _GET(name)\
    zxy_get_params(context, (name))

extern char * 
read_tpl(
    zxy_request_context *context,
    const char *tpl_name
);

extern void
free_tpl(
    const char *tpl_content
);

//common `action` method to return plain text.
extern int 
return_content(
    zxy_request_context *context,
    const std::string& content
);

//common `action` method to return json data
extern int 
return_json(
    zxy_request_context *context,
    const std::string& content
);

extern int 
return_jsonp(
    zxy_request_context *context,
    const std::string& content
);

extern int
return_file(
    zxy_request_context *context,
    const std::string& file_path
    );

extern int
return_redirect(
    zxy_request_context *context,
    const std::string& url
    );

extern zxy_user_platform 
get_user_platform(
        zxy_request_context *context
        );

extern std::string
ucfirst(
        const std::string& str
       );

enum quotestyle
{
    ENT_COMPAT,
    ENT_QUOTES,
    ENT_NOQUOTES
};

extern std::string
htmlentities(
        const std::string& str,
        quotestyle q_style,
        const std::string& char_set
        );

extern std::string
get_params_from_query(
        zxy_request_context *context,
        const std::string& name
        );

extern std::string
get_params_from_post(
        zxy_request_context *context,
        const std::string& name
        );
extern bool
is_ios(
        zxy_request_context *context
      );

extern std::string
formatsizeunits(
        float size_bytes
        );

extern std::string
formatduration(
        float time_sec
        );

extern std::string
basename(
        const std::string& file_path
        );

extern std::string
strtolower(
        const std::string& str
        );
extern std::string
GetMac(zxy_request_context*);


namespace std
{

extern std::string
to_string(
        const std::string& str
        );

#ifdef __UCLIBC__
static inline std::string 
    to_string(
            int n
            )
{
    char str[32];
    snprintf(str, sizeof(str), "%d", n);
    return std::string(str);
}
#endif

}

extern bool 
init_language_package(const std::string&);

extern std::string 
L(const std::string& key);

#endif
