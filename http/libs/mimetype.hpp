#ifndef LIBS_MIMETYPE_INCLUDED
#define LIBS_MIMETYPE_INCLUDED

#include <string>

//get mimetype by file extension
const std::string
get_mime_type (
    const char *file_ext
    );

void 
init_mimetype_map();

void
destroy_mimetype_map();

#endif
