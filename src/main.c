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
#include <signal.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "command.h"

/* Shell built-ins */
void change_directory(command_t* cmd);

int main(int argc, char* argv[])
{
    char *user = getenv("USER");
    char *base_dir = basename(getcwd(NULL, 256));
    char cmdbuff[BUF_LEN];
    char *cmdline = cmdbuff;
    command_t cmd;
    pid_t child;

    if(argc != 1) {
        fprintf(stderr, "arguments not yet supported!\n");
        return EXIT_FAILURE;
    }
    /* WIP: Signal handling is currently broken */
    /* Shell shouldn't exit when a child process is killed, so we mask
     * SIGINT, SIGTSTP, and SIGQUIT and will unmask them in the child fork
     */

    while(1) {
        printf("[%s] %s $ ", user, base_dir);
        if(fgets(cmdline, BUF_LEN, stdin) == NULL) {
            putchar('\n');
            return EXIT_SUCCESS;
        }

        /* ignore leading blanks in cmdline */
        while(isblank(*cmdline))
            cmdline++;

        /* ignore blank lines */
        if(strcmp(cmdline, "\n")) {
            parse_command(cmdline, &cmd);

            if(!strcmp(cmd.name, "cd"))
                change_directory(&cmd);
            else if(!strcmp(cmd.name, "exit")) {
                free_command(&cmd);
                break;
            } else {
                child = fork();

                if(child == 0) {
                    /* give child its own process group */
                    setpgid(0, 0);

                    execvp(cmd.name, cmd.argv);
                    fprintf(stderr, "Error executing command %s: %s\n",
                            cmd.name, strerror(errno));
                } else if (child > 0) {
                    /* give child its own process group */
                    setpgid(child, child);
                    wait(NULL);
                } else {
                    fprintf(stderr, "Error forking child process: %s\n",
                            strerror(errno));
                    return EXIT_FAILURE;
                }
            }

        free_command(&cmd);
        user = getenv("USER");
        base_dir = basename(getcwd(NULL, 256));
        }
    }

    return EXIT_SUCCESS;
}


/**
 * @name    change_directory
 * @arg     cmd, command pointer
 *
 * @brief   change working directory
 *
 * @post    if argv[1] is a path, change working directory to that path
 * @post    if argv[1] is NULL, change to home directory
 */

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
