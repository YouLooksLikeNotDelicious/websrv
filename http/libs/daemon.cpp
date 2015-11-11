#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>

char* make_daemon_init(int workdir, int stream, const char *username)
{
    struct passwd pwd;
    struct passwd *result;
    char *buf;
    size_t bufsize;

    bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if ( bufsize == (unsigned)-1 ) {
        bufsize = 16384;
    }

    buf = (char *)malloc(bufsize);
    if (buf == NULL) {
        return NULL;
    }

    getpwnam_r(username, &pwd, buf, bufsize, &result);
    if ( result == NULL ) {
        free(buf);
        return NULL;
    }

    setgid(pwd.pw_gid);
    setuid(pwd.pw_uid);
    daemon(workdir, stream);
    return buf;
}

void make_daemon_release(char *buf)
{
    free(buf);
}
