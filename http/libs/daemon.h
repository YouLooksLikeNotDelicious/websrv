#ifndef _DAEMON_H_INCLUDE_
#define _DAEMON_H_INCLUDE_

#define CHANGE_WORKDIR_TO_ROOT 0
#define KEEP_WORKDIR 1
#define CLOSE_STD_STREAM 0
#define KEEP_STD_STREAM 1

char* make_daemon_init(int workdir = KEEP_WORKDIR, int stream = CLOSE_STD_STREAM, const char *username = "nobody");

void make_daemon_release(char *buf);

#endif
