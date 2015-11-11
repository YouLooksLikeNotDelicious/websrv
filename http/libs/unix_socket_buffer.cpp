#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "unix_socket_buffer.h"

struct msg_inner_buf {
    int len;
    char buf[0];
} __attribute__((packed));

struct msg *append_msg(struct msg *msg, void *data, int len)
{
    struct msg_inner_buf * buf = (struct msg_inner_buf *)(msg->inner_buf);
    if ( msg->len > len + msg->used ) {
        memcpy(buf->buf, data, len);
    }
    else {
        while ( msg->len <= len + msg->used ) {
            msg->len *= 2;
        }
        void *tmp = realloc(msg, msg->len + sizeof(struct msg) + sizeof(struct msg_inner_buf));
        if ( tmp == NULL ) {
            assert(0);
        }
        msg = (struct msg *)tmp;
        buf = (struct msg_inner_buf *)msg->inner_buf;
        memcpy(buf->buf, data, len);
    }
    buf->len += len;
    msg->used += len;
    return msg;
}

struct msg *init_msg()
{
    const int len = 2048;
    struct msg *msg = (struct msg *)malloc(sizeof(struct msg) + len + sizeof(struct msg_inner_buf));
    msg->len = len;
    msg->used = 0;
    struct msg_inner_buf *buf = (struct msg_inner_buf*)msg->inner_buf;
    buf->len = 0;
    return msg;
}

struct msg *clear_msg(struct msg *msg)
{
    msg->used = 0;
    struct msg_inner_buf *buf = (struct msg_inner_buf*)msg->inner_buf;
    buf->len = 0;
    return msg;
}

void release_msg(struct msg *msg)
{
    free(msg);
}
