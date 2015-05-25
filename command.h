#ifndef __COMMAND_H__
#define __COMMAND_H__

#define BUF_LEN 256
#define MAX_ARGS BUF_LEN/2
#define WHITESPACE " \t\n"

typedef struct {
    char* name;
    int argc;
    char* argv[MAX_ARGS+1];
} command_t;

int parse_command(char* cmdline, command_t* cmd);

#endif
