/* llama-shell -- a linux shell */
/* command.h -- header for command.c */

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

#ifndef _COMMAND_H
#define _COMMAND_H

#define BUF_LEN 256
#define MAX_ARGS BUF_LEN/2
#define WHITESPACE " \t\n"

#if HAVE_LIBINTL_H == 1
#include <libintl.h>
#  define _(msg) gettext(msg)
#else
#  define _(msg) msg
#endif

typedef struct {
    char* name;              /* command name, same as argv[0] */
    int argc;                /* number of arguments, including name */
    char* argv[MAX_ARGS+1];  /* array of arguments, argv[0] is name */
    int infile;              /* the input stream for the process */
    int outfile;             /* the output stream for the process */
    int errfile;             /* the error stream for the process */
} command_t;

int parse_command(char* cmdline, command_t* cmd);
void free_command(command_t* cmd);

#endif
