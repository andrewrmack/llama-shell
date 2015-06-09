/* llama-shell -- a linux shell */
/* main.c -- main shell loop */

/* Author: Andrew Mack
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
#include <termios.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "command.h"
#include "getargs.h"

/* Shell built-ins */
void change_directory(command_t* cmd);

int main(int argc, char* argv[])
{
    char *user;
    char *cwd;
    char *base_dir;
    char cmdbuff[BUF_LEN];
    char *cmdline;
    command_t cmd;
    pid_t child, shell_pgid;
    struct termios shell_tmodes;
    int status;
    int arg_status;

    if((arg_status = getargs(argc, argv)) == 1)
        return EXIT_SUCCESS;

    /* Get environment for prompt */
    user = getenv("USER");
    cwd = getcwd(NULL, 256);
    base_dir = basename(cwd);

    cmdline = cmdbuff;

    shell_pgid = getpid();
    while(tcgetpgrp(STDIN_FILENO) != (shell_pgid = getpgrp()))
        kill(- shell_pgid, SIGTTIN);

    /* mask signals so shell doesn't quit when killing child */
    sigset_t signals;
    sigemptyset(&signals);

    sigaddset(&signals, SIGINT);
    sigaddset(&signals, SIGTSTP);
    sigaddset(&signals, SIGQUIT);
    sigaddset(&signals, SIGTTOU);
    sigaddset(&signals, SIGTTIN);

    sigprocmask(SIG_BLOCK, &signals, NULL);

    /* set up shell's process */
    shell_pgid = getpid();
    if(setpgid(shell_pgid, shell_pgid) < 0) {
        fprintf(stderr, "%s: could not put shell in own group: %s\n",
                argv[0], strerror(errno));
        return EXIT_FAILURE;
    }

    tcsetpgrp(STDIN_FILENO, shell_pgid);
    tcgetattr(STDIN_FILENO, &shell_tmodes);

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
            if(parse_command(cmdline, &cmd) == EXIT_FAILURE)
                break;

            if(!strcmp(cmd.name, "cd")) {
                change_directory(&cmd);
                free_command(&cmd);
            } else if(!strcmp(cmd.name, "exit")) {
                free_command(&cmd);
                break;
            } else {
                child = fork();

                if(child == 0) {
                    /* give child its own process group */
                    setpgid(0, 0);

                    dup2(cmd.infile, STDIN_FILENO);
                    dup2(cmd.outfile, STDOUT_FILENO);
                    dup2(cmd.errfile, STDERR_FILENO);

                    sigprocmask(SIG_UNBLOCK, &signals, NULL);
                    execvp(cmd.name, cmd.argv);
                    fprintf(stderr, "Error executing command %s: %s\n",
                            cmd.name, strerror(errno));
                } else if (child > 0) {
                    /* give child its own process group */
                    setpgid(child, child);
                    tcsetpgrp(STDIN_FILENO, child);
                    waitpid(child, &status, WUNTRACED);
                    tcsetpgrp(STDIN_FILENO, shell_pgid);
                    tcsetattr(STDIN_FILENO, TCSADRAIN, &shell_tmodes);
                    if(!WIFEXITED(status))
                        putchar('\n');
                    if(WIFSTOPPED(status)) {
                        kill(- child, SIGINT);
                        printf("[%d]+ Stopped\n", child);
                    }
                } else {
                    fprintf(stderr, "Error forking child process: %s\n",
                            strerror(errno));
                    return EXIT_FAILURE;
                }
            }

        free_command(&cmd);
        free(cwd);
        user = getenv("USER");
        cwd = getcwd(NULL, 256);
        base_dir = basename(cwd);
        }
    }

    free(cwd);
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
