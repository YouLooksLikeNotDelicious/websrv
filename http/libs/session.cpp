#include <map>
#include <string>
#include <cstring>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#include "session.hpp"
#include "web.hpp"
#include "../evhtp.h"
#include "logger.hpp"

static session_container_type sessions;
static struct session_queue_record *session_visit_record_header;
//static pthread_mutex_t mutex;
//static pthread_mutexattr_t mutex_attr;

void
session_init()
{
    session_visit_record_header  = new session_queue_record();
    session_visit_record_header->session_id = "";
    session_visit_record_header->next = session_visit_record_header;
    session_visit_record_header->prev = session_visit_record_header;

    //pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);
    //pthread_mutex_init(&mutex, &mutex_attr);
}

static void
remove_visit_record_of_session(struct session_type *session)
{
    struct session_queue_record *record = session->visit_record;
    if ( record == NULL )
    {
        abort();
    }
    else
    {
        record->prev->next = record->next;
        record->next->prev = record->prev;
        record->next = NULL;
        record->prev = NULL;
    }
}

static void
clear_a_session(session_type *&session)
{
    log("clear session %s", session->session_id.c_str());
    if ( session == NULL )
    {
        abort();
        return;
    }
    session_values_container_type *session_values = session->session_values;
    session_values_container_type::iterator item_it;
    for (item_it = session_values->begin(); item_it != session_values->end(); ++item_it)
    {
        session_value *value = item_it->second;
        if ( value == NULL )
        {
            continue;
        }
        dispose_func_callback dispose_func = value->dispose_func;
        if ( dispose_func != NULL )
        {
            dispose_func(value->value);
        }
        delete value;
    }
    session->session_values->clear();
    delete session->session_values;
    remove_visit_record_of_session(session);
    delete session->visit_record;
    //pthread_mutex_destroy(&(session->mutex));
    delete session;
    session = NULL;
}

void 
session_destroy_all()
{
    session_container_type::iterator it;
    for (it = sessions.begin(); it != sessions.end(); ++it)
    {
        clear_a_session(it->second);
    }
    sessions.clear();
    delete session_visit_record_header;
    session_visit_record_header = NULL;
    //pthread_mutex_destroy(&mutex);
}

void 
session_destroy(const std::string& session_id)
{
    log("begin session destroy");
    //pthread_mutex_lock(&mutex);
    log("session destroy");

    session_container_type::iterator it = sessions.find(session_id);
    if ( it == sessions.end() )
    {
        return;
    }
    session_type *session = sessions[session_id];
    clear_a_session(session);

    std::map<std::string, session_type* >::iterator itDel = sessions.find(session_id);

    if ( itDel != sessions.end() )
    {
        itDel = sessions.erase(itDel);
    }

    // sessions.erase(session_id);

    //pthread_mutex_unlock(&mutex);
}


void
session_clean_timeout(
    const zxy_site_config *config
    )
{
    session_queue_record *record = session_visit_record_header->next;
    time_t now = time(NULL);
    while ( record != session_visit_record_header )
    {
        if ( record->status == online )
        {
            break;
        }
        struct session_queue_record *next = record->next;
        time_t last_visit_time = record->last_visit_time;
        double seconds = difftime(now, last_visit_time);
        if ( seconds < config->session_timeout )
        {
            break;
        }
        else
        {
            session_destroy(record->session_id); 
        }
        record = next;
    }
}

static const std::string
generate_session_id()
{
    int fd = open("/proc/sys/kernel/random/uuid", O_RDONLY);

    char session_id[SESSION_ID_LENGTH];
    if ( read(fd, session_id, SESSION_ID_LENGTH - 1) == -1 )
    {
        abort();
    }
    session_id[SESSION_ID_LENGTH - 1] = 0;
    if ( close(fd) == -1 )
    {
        abort();
    }
    return session_id;
}

const static std::string
get_session_id_from_cookie(
        zxy_request_context *context
        )
{
    evhtp_header_t *cookies_header = NULL;
    std::string session_id;
    cookies_header = evhtp_headers_find_header(context->request->headers_in, "Cookie");
    if ( cookies_header == NULL )
    {
        return session_id;
    }
    std::string cookie_val = cookies_header->val;
    size_t pos = cookie_val.find(SESSION_COOKIE_NAME"=");
    if ( pos == std::string::npos )
    {
        return session_id;
    }
    size_t end = cookie_val.find("; ", pos);
    if ( end == std::string::npos )
    {
        end = cookie_val.length() - 1;
    }
    size_t start = pos + std::strlen(SESSION_COOKIE_NAME"=");
    size_t len = end - start + 1;
    return cookie_val.substr(start, len);
}

static session_type *
get_current_session_from_cookie(
    struct zxy_request_context *context
    )
{
    std::string session_id = get_session_id_from_cookie(context);
    if ( session_id.empty() ) 
    {
        return NULL;
    }
    else
    {
        session_container_type::iterator it = sessions.find(session_id);
        if ( it == sessions.end() )
        {
            return NULL;
        }
        return it->second;
    }
}

static session_type *
get_current_session(
    struct zxy_request_context *context
    )
{
    session_container_type::iterator it = sessions.find(context->session_id);

    if ( it != sessions.end() )
    {
        return it->second;
    }
    return NULL;
}

void 
session_start(
    struct zxy_request_context *context
    )
{
    //log("session start");
    //pthread_mutex_lock(&mutex);

    session_clean_timeout(context->http_server->site_config);

    session_type *current_session = NULL;
    struct session_queue_record *visit_record;

    current_session = get_current_session_from_cookie(context);

    if ( current_session == NULL )
    {
        std::string session_id = generate_session_id();

        log("New session %s", session_id.c_str());

        current_session = new session_type();
        current_session->session_id = session_id;
        current_session->session_values = new session_values_container_type();
        current_session->is_new = true;
        //pthread_mutex_init(&(current_session->mutex), NULL);
        sessions[session_id] = current_session;

        //set session id to th cookie
        std::string cookies(SESSION_COOKIE_NAME"=");
        cookies = cookies + session_id + "; path=/; HttpOnly";
        zxy_add_http_header(context, "Set-Cookie", cookies);

        visit_record = new session_queue_record();
        visit_record->session_id = current_session->session_id;
        visit_record->status = online;
        current_session->visit_record = visit_record;
    }
    else 
    {
        visit_record = current_session->visit_record;
        visit_record->status = online;
    }
    context->session_id = current_session->session_id;

    visit_record->last_visit_time = time(NULL);
    // A process per session
    //pthread_mutex_lock(&(current_session->mutex));

    // 调整当前访问记录的位置到链末端

    if ( !current_session->is_new )
    {
        visit_record->prev->next = visit_record->next;
        visit_record->next->prev = visit_record->prev;
    }

    session_visit_record_header->prev->next = visit_record;
    visit_record->prev = session_visit_record_header->prev;

    visit_record->next = session_visit_record_header;
    session_visit_record_header->prev = visit_record;

    //pthread_mutex_unlock(&mutex);
}


void
session_end(
    struct zxy_request_context *context
    )
{
    //log("session end");
    session_type *current_session = get_current_session(context);
    if ( current_session == NULL )
    {
        // abort();
    }
    else
    {
        current_session->is_new = false;
        current_session->visit_record->status = offline;
        //pthread_mutex_unlock(&(current_session->mutex));
    }
}

void
session_set(
    zxy_request_context *context,
    const std::string& key,
    void *val,
    dispose_func_callback dispose_func)
{
    log("%s", key.c_str());
    session_type *current_session = get_current_session(context);
    if ( current_session == NULL )
    {
        abort();
    }
    else
    {
        session_values_container_type *session_values = current_session->session_values;
        session_value *value = new session_value();
        value->value = val;
        value->dispose_func = dispose_func;
        //Remove original session value
        session_values_container_type::iterator it = session_values->find(key);
        if ( it != session_values->end() )
        {
            it->second->dispose_func(it->second->value);
        }
        (*session_values)[key] = value;
    }
}

void session_set_ex(const std::string& session_id, const std::string& key, void* val, dispose_func_callback dispose_func)
{
    zxy_request_context Context;
    Context.session_id = session_id;

    session_type* cur_session = get_current_session(&Context);

    struct session_queue_record *visit_record;

    if ( cur_session == NULL )
    {
        cur_session = new session_type;
        cur_session->session_id = session_id;
        cur_session->session_values = new session_values_container_type();
        cur_session->is_new = true;
        //pthread_mutex_init(&(current_session->mutex), NULL);
        sessions[session_id] = cur_session;
        
        /*
        //set session id to th cookie
        std::string cookies(SESSION_COOKIE_NAME"=");
        cookies = cookies + session_id + "; path=/; HttpOnly";
        zxy_add_http_header(&Context, "Set-Cookie", cookies);
        */
        visit_record = new session_queue_record();
        visit_record->session_id = cur_session->session_id;
        visit_record->status = online;
        cur_session->visit_record = visit_record;
    }
    else
    {
        visit_record = cur_session->visit_record;
        visit_record->status = online;
    }

    visit_record->last_visit_time = time(NULL);

    if ( !cur_session->is_new )
    {
        visit_record->prev->next = visit_record->next;
        visit_record->next->prev = visit_record->prev;
    }
/*
    session_visit_record_header->prev->next = visit_record;
    visit_record->prev = session_visit_record_header->prev;

    visit_record->next = session_visit_record_header;
    session_visit_record_header->prev = visit_record;
*/
    session_values_container_type *session_values = cur_session->session_values;
    if ( session_values != NULL )
    {
        session_value *value = new session_value();
        if ( value != NULL )
        {
            value->value = val;
            value->dispose_func = dispose_func;
            //Remove original session value
            session_values_container_type::iterator it = session_values->find(key);
            if ( it != session_values->end() )
            {
                it->second->dispose_func(it->second->value);
            }
            (*session_values)[key] = value;
        }
    }
}

void *
session_get(
    struct zxy_request_context *context,
    const std::string& key)
{
    session_type *current_session = get_current_session(context);
    if ( current_session == NULL )
    {
        abort();
    }
    else
    {
        session_values_container_type::iterator it = current_session->session_values->find(key);
        if ( it == current_session->session_values->end() )
        {
            return NULL;
        }
        else
        {
            return it->second->value;
        }
    }
}

const std::string
get_session_id(
    struct zxy_request_context *context
    )
{
    session_type *current_session = get_current_session(context);
    if ( current_session == NULL )
    {
        abort();
        return NULL;
    }
    else
    {
        return current_session->session_id;
    }
}

bool
is_new_session(
    struct zxy_request_context *context
    )
{
    session_type *current_session = get_current_session(context);
    if ( current_session == NULL )
    {
        abort();
        return false;
    }
    else
    {
        return current_session->is_new;
    }
}
