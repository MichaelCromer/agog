/* ============================================================
 *  AGOG 
 * ------------------------------------------------------------
 *
 *  LICENSE: MIT
 *  DESCRIPTION: A command-line tool for project management.
 *
 *  AUTHOR: Michael Cromer
 *  DATE: 2024-04-02
 *  VERSION: 0.1
 *
 * ------------------------------------------------------------
 *
 *  EXAMPLE USAGE:
 *
 *    agog project --create [project]
 *    agog clockon [project]
 *    agog task --add [task]
 *    agog log 1h [task] --message "I did this"
 * 
 * ============================================================
 */

/* ============================================================
 *  SETUP
 * ============================================================
 */

/* ------------------------------------------------------------
 * INCLUDES
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------
 * DEFINES
 * */

#define AGOG_VERSION "0.1"

/* ------------------------------------------------------------
 * PROTOTYPES
 * */

void agog_short_help(void);
void agog_help(void);

/* ============================================================
 *  MAIN
 * ============================================================
 */


int main(int argc, char *argv[])
{
    if (argc < 2) {
        agog_short_help();
        return EXIT_SUCCESS;
    }

    char *command = argv[1];

    if (strcmp(command, "help") == 0) {
        agog_help();
    } else {
        printf("Unrecognised command %s.\n", command);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


void agog_short_help()
{
    printf("============================================================\n");
    printf(" agog - a command line tool for time and project management \n");
    printf("------------------------------------------------------------\n");
    printf(" version " AGOG_VERSION ". try \'agog help\' for more  \n");
    printf("============================================================\n");
    return;
}

void agogo_help()
{
    printf("============================================================\n");
    printf("  Usage: agog [COMMAND] [OPTIONS]                           \n");
    printf("------------------------------------------------------------\n");
    printf("  COMMANDS:                                                 \n");
    printf("    project - list or interact with active projects         \n");
    printf("============================================================\n");
    return;
}
