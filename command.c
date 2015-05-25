#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "command.h"

int parse_command(char *cmdline, command_t* cmd)
{
    int argc = 0;
    char* word;

    word = strsep(&cmdline, WHITESPACE);

    while(word) {
        if(strlen(word)) {
            cmd->argv[argc] = (char*) malloc((strlen(word)+1)*sizeof(char));

            if(cmd->argv[argc] == NULL) {
                fprintf(stderr,
                        "Error allocating memory for argument \"%s\": %s\n",
                        word, strerror(errno));
                return EXIT_FAILURE;
            }

            strcpy(cmd->argv[argc], word);//, BUF_LEN);
            argc++;
        }
        word = strsep(&cmdline, WHITESPACE);
    }

    cmd->argv[argc] = NULL;

    cmd->argc = argc;
    cmd->name = (char*) malloc((strlen(cmd->argv[0])+1)*sizeof(char));

    if (NULL == cmd->name) {
        fprintf(stderr, "Error allocating memory for command name %s: %s\n",
                cmd->argv[0], strerror(errno));
        return EXIT_FAILURE;
    }

    strcpy(cmd->name, cmd->argv[0]);//, BUF_LEN);

    return EXIT_SUCCESS;
}
