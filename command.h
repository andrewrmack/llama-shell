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
