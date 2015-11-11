#ifndef _CMD_OPT_H__
#define _CMD_OPT_H__

#include <string>

class option {
public:
    bool daemon;
    std::string config;
public:
    option() : daemon(false), config("site.config") {}
};

int parser_cmd(option *op, int argc, char **argv);

#endif
