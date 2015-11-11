#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
//#include <time.h>
#include <locale.h>
#include <errno.h>

#ifndef LOGGER_INCLUDED
#define LOGGER_INCLUDED

#include "logger.h"

#define log(...) SLOG(DEBUG, ##__VA_ARGS__)
#define logerr(...) SLOG(FATAL, ##__VA_ARGS__)

void init_log(const char *path, const char *filename, int log_level);
void release_log();

#if 0

#define print_now_local() \
    do {\
        timeval now;\
        gettimeofday(&now, NULL);\
        tm now_local;\
        localtime_r(&now.tv_sec, &now_local);\
        char buff[30];\
        strftime(buff, sizeof(buff), "%F %T", &now_local);\
        fprintf(__out__, "%s.%ld\t", buff, now.tv_usec);\
    } while (0)

#define print_error(msg)\
    do { \
    fprintf(__out__, "%s: \n\tcode: %d, msg: %s. At %d of %s in function %s", msg, errno, strerror(errno), __LINE__, __FILE__, __func__);\
    } while (0)

#ifdef ZXY_LOG_DEBUG

#define log(format,...)\
    do {\
        FILE *__out__ = fdopen(STDOUT_FILENO, "w");\
        print_now_local();\
        fprintf(__out__, format, ## __VA_ARGS__);\
        fprintf(__out__, "\n");\
    } while(0)

#define logerr(msg)\
    do {\
        FILE *__out__ = fdopen(STDERR_FILENO, "w");\
        print_now_local();\
        print_error(msg);\
        fprintf(__out__, "\n");\
    } while(0)

#else

#define log(format,...)

#define logerr(msg)\
    do {\
        FILE *__out__ = fdopen(STDERR_FILENO, "w");\
        print_now_local();\
        print_error(msg);\
        fprintf(__out__, "\n");\
    } while(0)

#endif
#endif

#endif
