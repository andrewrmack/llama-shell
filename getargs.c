#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "getargs.h"
#include "version.h"

/**
 * @brief   parses command line arguments to llama-shell
 *
 * @note    this function is derived almost exactly from the getopt_long
 *          example in the GNU C Library Manual
 *
 * @param   argc, the argc passed to main by the terminal
 * @param   argv, the argv passed to main by the terminal
 *
 * @pre     no additional
 *
 * @return  int
 *
 * @post    if version or help flags were invoked, call procedure to print
 *          relevant info, then return 1.
 * @post    otherwise, return 0.
 */
int getargs(int argc, char *argv[])
{
    while(1) {

        int c;

        static struct option long_options[] =
            {
                {"version", no_argument, 0, 'v'},
                {"help",    no_argument, 0, 'h'},
                {0, 0, 0, 0}
            };

        int option_index = 0;

        c = getopt_long(argc, argv, "vh", long_options, &option_index);

        if(c == -1)
            break;

        switch(c) {
            case 'v':
                printversion();
                return 1;
            case 'h':
                printhelp();
                return 1;
            default:
                return -1;

        }
    }

    return 0;
}

void printhelp()
{
    printf("llama-shell: a minimal Linux shell\n"
           "\tTo use llama-shell, call llama-shell with no arguments\n"
           "\tTo check version, call llama-shell with -v"
           " or --version option\n"
           "\tTo print this help, call llama-shell with -h or --help"
           " option\n");
    printf("Report bugs to: andrewrmack@gmail.com\n"
           "llama-shell home page:"
           " <https://github.com/andrewrmack/llama-shell>\n");
}

void printversion()
{
    printf("llama-shell, version %s (%s)\n", VERSION, MACHTYPE);
    printf("License GPLv2+: GNU GPL version 2 or later"
           " <http://gnu.org/licenses/gpl-2.0.html>\n");
    printf("\nThis if free software;"
           " you are free to change and redistribute it.\n");
    printf("There is NO WARRANTY, to the extent permitted by the law\n");
}
