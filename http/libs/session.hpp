#ifndef SESSION_INCLUDED
#define SESSION_INCLUDED

#include <map>
#include <string>
#include <pthread.h>

#include "web.hpp"

#define SESSION_ID_LENGTH 37
#define SESSION_COOKIE_NAME "ZXYSID"

typedef void (*dispose_func_callback)(void *);

struct session_value 
{
    dispose_func_callback dispose_func;
    void *value;
};
typedef std::map<std::string, session_value *> session_values_container_type;

enum session_status
{
    online = 1,
    offline = 0
};

struct session_queue_record
{
    std::string session_id;
    session_status status;
    time_t last_visit_time;
    struct session_queue_record *next;
    struct session_queue_record *prev;
};

struct session_type 
{
    std::string session_id;
    struct session_queue_record *visit_record;
    bool is_new;
    session_values_container_type *session_values;
    //pthread_mutex_t mutex;
};

typedef std::map<std::string, session_type* > session_container_type;

extern void
session_init();

extern void
session_destroy_all();

extern void
session_destroy(const std::string& session_id);


extern void
session_start(
    zxy_request_context *context
    );

extern void
session_end(
    struct zxy_request_context *context
    );

extern void
session_set(
    struct zxy_request_context *context, 
    const std::string& key, 
    void *val,
    dispose_func_callback dispose_func
    );

extern void
session_set_ex(
        const std::string& session_id,
        const std::string& key,
        void* val,
        dispose_func_callback dispose_func
        );

extern void *
session_get(
    struct zxy_request_context *context, 
    const std::string& key
    );

extern const std::string
get_session_id(
    struct zxy_request_context *context
    );

extern bool
is_new_session(
    struct zxy_request_context *context
    );

#endif
