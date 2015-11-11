#ifndef __UNIX_SOCKET_BUFFER_H__
#define __UNIX_SOCKET_BUFFER_H__

struct msg {
    int len;
    int used;
    char inner_buf[0];
} __attribute__((packed));

struct msg *init_msg();
struct msg *clear_msg(struct msg *msg);
struct msg *append_msg(struct msg *msg, void *data, int len);

void release_msg(struct msg *msg);

#endif
