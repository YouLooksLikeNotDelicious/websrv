#include <cstring>
#include <string>
#include <map>

#include "mimetype.hpp"
#include "logger.hpp"

static char dot = '.';

struct ext_mimetype_pair {
    const char *ext;
    const char *mimetype;
};

struct ext_mimetype_pair mimetypes[] = {
    {".js", "application/x-javascript; charset=utf-8"},
    {".css", "text/css; charset=utf-8"},
    {".gif", "image/gif"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".html", "text/html; charset=utf-8"},
    {".htm", "text/html; charset=utf-8"},
    {".png", "image/png"},
    {".zip", "application/zip"},
    {".mp3", "audio/mp3"},
    {".mp4", "video/mp4"}
};

static std::map<std::string, std::string> mimetype_map;

void 
init_mimetype_map()
{
    unsigned int i;
    for (i = 0; i < sizeof(mimetypes) / sizeof(mimetypes[0]); i++)
    {
        mimetype_map[mimetypes[i].ext] = mimetypes[i].mimetype;
    }
}

void 
destroy_mimetype_map()
{
    mimetype_map.clear();
}

const static char *
get_file_ext(
    const char *file_path
    )
{
    return std::strrchr(file_path, dot);
}

const std::string get_mime_type(
    const char *file_path
    )
{
    const char *ext = get_file_ext(file_path);
    if ( ext == NULL )
    {
        return "application/octet-stream";
    }

    std::map<std::string, std::string>::iterator it = mimetype_map.find(ext);
    if (it != mimetype_map.end()) 
    {
        return it->second;
    }

    return "application/octet-stream";
}
