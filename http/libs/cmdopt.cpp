#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <unistd.h>
#include "cmdopt.h"

static const char *options = "dc:v";
static const char* version = "1.0.125";

int parser_cmd(option *op, int argc , char **argv)
{
    int opt = 0;
    opt = getopt(argc, argv, options);
    while ( opt != -1 ) {
        switch ( opt ) {
        case 'd':
            op->daemon = true;
            break;
        case 'c':
            op->config = optarg;
            break;
        case 'v':
            printf("%s\r\n", version);
            exit(0);
            // return version string and exit process 
            break;
        default:
            break;
        }
        opt = getopt(argc, argv, options);
    }
    return 0;
}
