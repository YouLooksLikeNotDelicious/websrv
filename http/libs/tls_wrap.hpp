#ifndef __TLS_WRAP_H__
#define __TLS_WRAP_H__

#include <pthread.h>
#include <stdlib.h>

static pthread_key_t key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

typedef void (*destory_tls_data)(void *ptr);

static destory_tls_data destructor;

static void make_key()
{
    pthread_key_create(&key, destructor);
}

#endif
