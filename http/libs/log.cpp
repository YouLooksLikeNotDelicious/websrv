#include "logger.hpp"

void init_log(const char *path, const char *filename, int level)
{
    slog_level log_level;

    switch(level) {
        case 0:  
            log_level = SLOG_LEVEL_DEBUG;
            break;
        case 1:
            log_level = SLOG_LEVEL_INFO;
            break;
        case 2:
            log_level = SLOG_LEVEL_WARN;
            break;
        case 3:
            log_level = SLOG_LEVEL_ERROR;
            break;
        case 4:
            log_level = SLOG_LEVEL_FATAL;
            break;
        default:
            log_level = SLOG_LEVEL_ERROR;
        }   
    slog_open(path, filename, log_level, SLOG_MB(100));
}

void release_log()
{
    slog_close();
}
