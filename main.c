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
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "command.h"

/* TODO: Should this return an int and check value? */
void change_directory(command_t* cmd)
{
    if(cmd->argc == 1) {
        char *home_dir = getenv("HOME");
        chdir(home_dir);
    } else if (cmd->argc == 2)
        chdir(cmd->argv[1]);
    else
        fprintf(stderr, "cd: too many arguments!\n");
}
int main(int argc, char* argv[])
{
    char cmdbuff[BUF_LEN];
    /* TODO: change this when implementing pipes, dependend cmds, etc. */
    char *cmdline = cmdbuff;
    command_t cmd;
    int childpid;

    if(argc != 1) {
        /* TODO: Will parse here when args support is added */
        fprintf(stderr, "arguments not yet supported!\n");
        return EXIT_FAILURE;
    }

    while(1) {
        /* TODO: implement PS1 variable support */
        printf(">>> ");
        if(fgets(cmdline, BUF_LEN, stdin) == NULL) {
            putchar('\n');
            return EXIT_SUCCESS;
        }

        /* ignore leading blanks in cmdline */
        while(isblank(*cmdline))
            cmdline++;

        if(strcmp(cmdline, "\n")) {
            parse_command(cmdline, &cmd);

            if(!strcmp(cmd.name, "cd"))
                change_directory(&cmd);
            else {
                childpid = fork();

                if(childpid == 0) {
                    execvp(cmd.name, cmd.argv);
                    fprintf(stderr, "Error executing command %s: %s\n",
                            cmd.name, strerror(errno));
                } else if (childpid > 0)
                    wait(NULL);
                else {
                    fprintf(stderr, "Error forking child process: %s\n",
                            strerror(errno));
                    return EXIT_FAILURE;
                }
            }
        }
    }

    return EXIT_SUCCESS;
}
