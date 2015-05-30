/* llama-shell -- a linux shell */

/* Copyright (C) 2015 Andrew Mack
 *
 * This file is part of llama-shell
 *
 * llama-shell is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the license, or
 * (at your option) any later version.
 *
 * llama-shell is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have recieved a copy of the GNU General Public License
 * along with llama-shell. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "command.h"

/**
 * @name    parse_command
 * @arg     cmdline, string
 * @arg     cmd, command pointer
 *
 * @brief   parses a string into command and arguments
 *
 * @pre     cmdline has had already been parsed for pipes and redirects
 *
 * @post    cmd contains information about the command contained in cmdline
 *
 * @note    the line is parsed and each word is passed directly to
 *          execvp(), thus, any redirects must be parsed out before the
 *          line is passed to parse_command
 */
int parse_command(char *cmdline, command_t *cmd)
{
    int argc = 0;
    char* word;

    cmd->infile = cmd->outfile = cmd->errfile = -1;

    word = strsep(&cmdline, WHITESPACE);

    while(word) {
        if(strlen(word)) {
            if(!strcmp(word, ">") || !strcmp(word, "1>")) {
                word = strsep(&cmdline, WHITESPACE);
                cmd->outfile = open(word, O_WRONLY|O_CREAT|O_TRUNC, 0755);
                if(cmd->outfile == -1) {
                    fprintf(stderr, "Invalid outfile %s\n", word);
                    return EXIT_FAILURE;
                }
            } else if(!strcmp(word, ">>") || !strcmp(word, "1>>")) {
                word = strsep(&cmdline, WHITESPACE);
                cmd->outfile = open(word, O_WRONLY|O_CREAT|O_APPEND, 0755);
                if(cmd->outfile == -1) {
                    fprintf(stderr, "Invalid outfile %s\n", word);
                    return EXIT_FAILURE;
                }
            } else if(!strcmp(word, "<")) {
                word = strsep(&cmdline, WHITESPACE);
                cmd->infile = open(word, O_RDONLY);
                if(cmd->infile == -1) {
                    fprintf(stderr, "Invalid infile %s\n", word);
                    return EXIT_FAILURE;
                }
            } else if(!strcmp(word, "2>")) {
                word = strsep(&cmdline, WHITESPACE);
                cmd->errfile = open(word, O_WRONLY|O_CREAT|O_TRUNC, 0755);
                if(cmd->errfile == -1) {
                    fprintf(stderr, "Invalid errfile %s\n", word);
                    return EXIT_FAILURE;
                }
            } else if(!strcmp(word, "2>>") || !strcmp(word, "1>")) {
                word = strsep(&cmdline, WHITESPACE);
                cmd->errfile = open(word, O_WRONLY|O_CREAT|O_APPEND, 0755);
                if(cmd->errfile == -1) {
                    fprintf(stderr, "Invalid errfile %s\n", word);
                    return EXIT_FAILURE;
                }
            } else if(!strcmp(word, "&>") || !strcmp(word, "1>")) {
                word = strsep(&cmdline, WHITESPACE);
                cmd->errfile = cmd->outfile = open(word, O_WRONLY|O_CREAT|O_TRUNC, 0755);
                if(cmd->outfile == -1) {
                    fprintf(stderr, "Invalid cmd->outfile %s\n", word);
                    return EXIT_FAILURE;
                }
            } else {

                cmd->argv[argc] = (char*) malloc((strlen(word)+1)*sizeof(char));

                if(cmd->argv[argc] == NULL) {
                    fprintf(stderr,
                            "Error allocating memory for argument \"%s\": %s\n",
                            word, strerror(errno));
                    return EXIT_FAILURE;
                }

                strcpy(cmd->argv[argc], word);
                argc++;
            }
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

    if (cmd->infile == -1)
        cmd->infile = STDIN_FILENO;
    if (cmd->outfile == -1)
        cmd->outfile = STDOUT_FILENO;
    if (cmd->errfile == -1)
        cmd->errfile = STDERR_FILENO;

    strcpy(cmd->name, cmd->argv[0]);

    return EXIT_SUCCESS;
}

/**
 * @name    free_command
 * @arg     cmd, command pointer
 *
 * @brief   frees a command structure
 *
 * @pre     cmd points to a command structure whose argv and name fields
 *          HAVE BEEN MALLOCED
 *
 * @post    cmd->name and all fields of cmd->argv are freed
 */
void free_command(command_t* cmd)
{
    int i;
    for(i = 0; i < cmd->argc; i++)
        free(cmd->argv[i]);

    free(cmd->name);
}
