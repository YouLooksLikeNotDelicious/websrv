#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "unix_socket.h"
#include "unix_socket_buffer.h"

#define UNIX_SOCK_FILE "/tmp/collecter.sock"

int unix_socket_init(const char *sockfile)
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if ( fd < 0 ) {
        return -1;
    }
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, sockfile);

    int ret = connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un));
    if ( ret < 0 ) {
        unix_socket_close(fd);
        return -1;
    }

    // unix_socket_setnonblock(fd);
    return fd;
}

int unix_socket_setnonblock(int fd)
{
    int raw_flag = fcntl(fd, F_GETFL, 0);
    if ( raw_flag < 0 ) {
        return -1;
    }
    int ret = fcntl(fd, F_SETFL, raw_flag | O_NONBLOCK);
    if ( ret < 0 ) {
        return -1;
    }
    return 0;
}

int unix_socket_sendmsg(int fd, struct msg *buf)
{
    int ret = send(fd, buf->inner_buf, buf->used + 4, 0);
    if ( ret < buf->len ) {
        return -1;
    }
    return 0;
}

int unix_socket_close(int fd)
{
    return close(fd);
}

