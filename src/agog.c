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
 * Includes
 * */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* ------------------------------------------------------------
 * Defines
 * */

#define AGOG_VERSION "0.1"
#define AGOG_DIR "/home/mcromer/tmp/agog/"
#define AGOG_PROJECTS AGOG_DIR"projects/"

/* ------------------------------------------------------------
 * Prototypes
 * */

// Help
void agog_short_help(void);
void agog_help(void);

// Project
void agog_project(int argc, char *argv[]);
void agog_project_list(void);
void agog_project_create(int argc, char *argv[]);


/* ============================================================
 *  FUNCTIONS
 * ============================================================
 */

/* ------------------------------------------------------------
 * Main
 * */

int main(int argc, char *argv[])
{
    if (argc < 2) {
        agog_short_help();
        return EXIT_SUCCESS;
    }

    char *command = argv[1];

    if (strcmp(command, "help") == 0) {
        agog_help();
    } else if (strcmp(command, "project") == 0) {
        agog_project(argc-1, &argv[1]);
    } else {
        printf("Unrecognised command %s.\n", command);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* ------------------------------------------------------------
 * Help
 * */

void agog_short_help()
{
    printf("============================================================\n");
    printf(" agog - a command line tool for time and project management \n");
    printf("------------------------------------------------------------\n");
    printf(" version " AGOG_VERSION ". try \'agog help\' for more  \n");
    printf("============================================================\n");
    return;
}

void agog_help()
{
    printf("============================================================\n");
    printf("  Usage: agog [COMMAND] [OPTIONS]                           \n");
    printf("------------------------------------------------------------\n");
    printf("  COMMANDS:                                                 \n");
    printf("    project - list or interact with active projects         \n");
    printf("============================================================\n");
    return;
}


/* ------------------------------------------------------------
 * Utils
 * */

void agog_setup()
{
    mkdir(AGOG_DIR, S_IRWXU);
    mkdir(AGOG_PROJECTS, S_IRWXU);
}

/* ------------------------------------------------------------
 * Project
 * */

void agog_project(int argc, char *argv[])
{
    if (argc < 2) {
        agog_project_list();
        return;
    }

    char *subcommand = argv[1];

    if (strcmp(subcommand, "-c") == 0 || strcmp(subcommand, "--create") == 0) {
        agog_project_create(argc-1, &argv[1]);
    } else {
        printf("Unrecognised option %s to agog-project.\n", subcommand);
    }
    return;
}

void agog_project_list()
{
    DIR *projects = opendir(AGOG_PROJECTS);
    struct dirent *project;

    while ((project = readdir(projects)) != NULL) {
        if (strncmp(project->d_name, ".", 1) == 0) {
            continue;
        }
        printf("%s\n", project->d_name);
    }

    closedir(projects);
    return;
}

void agog_project_create(int argc, char *argv[])
{
    if (argc < 2) {
        printf("agog-project-create requires argument : project name\n");
        exit(EXIT_FAILURE);
    }

    struct stat s;
    char *project = strcat(strdup(AGOG_PROJECTS), argv[1]);
    if (stat(project, &s) == 0 && S_ISDIR(s.st_mode)) {
        printf("Project %s already exists.\n", argv[1]);
        return;
    }
    mkdir(project, S_IRWXU);
}
