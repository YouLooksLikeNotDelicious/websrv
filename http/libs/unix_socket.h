#ifndef __UNIX_SOCKET_H__
#define __UNIX_SOCKET_H__

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/socket.h>

#define UNIX_SOCK_FILE "/tmp/collecter.sock"

struct msg;

int unix_socket_init(const char *sockfile);
int unix_socket_setnonblock(int fd);
int unix_socket_sendmsg(int fd, struct msg *buf);
int unix_socket_close(int fd);

#endif
