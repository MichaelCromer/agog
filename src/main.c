/* ============================================================
 *  AGOGO 
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
 *    agogo project --create [project]
 *    agogo clockon [project]
 *    agogo task --add [task]
 *    agogo status
 *    agogo log 1h [task] --message "I did this"
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

#ifndef AGOGO_H
#define AGOGO_H
#define AGOGO_VERSION "0.1"
#define AGOGO_DIR "/home/mcromer/programming/c/agogo/tmp/agogo"
#define AGOGO_PROJECTS_DIR AGOGO_DIR "/projects"
#endif

/* ------------------------------------------------------------
 * PROTOTYPES
 * */

// Setup
void agogo_setup();
void check_setup();

// Help and print
void print_short_help();

// Project commands
int agogo_project(int argc, char *argv[]);
void list_projects();
void create_project(char *project_name);
void destroy_project(char *project_name);


/* ============================================================
 *  MAIN
 * ============================================================
 */

int main(int argc, char *argv[])
{
  if (argc < 2) {
    print_short_help();
    return EXIT_SUCCESS;
  }

  char *first_command = argv[1];
  int exit_code = EXIT_SUCCESS;

  if (strcmp(first_command, "setup") == 0) {
    // I know this is a silly user experience, but it's just a demo
    agogo_setup();
  }
  check_setup();

  if (strcmp(first_command, "project") == 0) {
    exit_code = agogo_project(argc, argv);
  }

  return exit_code;
}

/* ============================================================
 *  SETUP
 * ============================================================
 */

void agogo_setup() {
  // create the agogo directory
  int status = system("mkdir -p " AGOGO_DIR "/projects");
  if (status != 0) {
    printf("Error: Could not create the agogo directory.\n");
    exit(EXIT_FAILURE);
  }
}

void check_setup() {
  // check that the agogo directory exists
  int status = system("test -d " AGOGO_DIR);
  if (status != 0) {
    printf("Error: Agogo directory not found. Please run `agogo setup` to create the directory.\n");
    exit(EXIT_FAILURE);
  }
}


/* ============================================================
 *  HELP/USAGE/VERSION
 * ============================================================
 */

void print_short_help() {
  printf("============================================================\n");
  printf("Agogo - A command-line tool for project management.\n");
  printf("Version: %s\n", AGOGO_VERSION);
  printf("============================================================\n");
}

void agogo_help() {

}

/* ============================================================
 *  PROJECT
 * ============================================================
 */

int agogo_project(int argc, char *argv[])
{
  if (argc < 3) {
    list_projects();
    return EXIT_SUCCESS;
  }

  char *sub_command = argv[2];

  if ((strcmp(sub_command, "--create") == 0) || (strcmp(sub_command, "-c") == 0)) {
    if (argc < 4) {
      printf("Error: Missing project name\n");
      return EXIT_FAILURE;
    }
    create_project(argv[3]);
  }

  if ((strcmp(sub_command, "--destroy") == 0) || (strcmp(sub_command, "-d") == 0)) {
    if (argc < 4) {
      printf("Error: Missing project name\n");
      return EXIT_FAILURE;
    }
    destroy_project(argv[3]);
  }

  return EXIT_SUCCESS;
}

void list_projects() 
{
  printf("Listing projects\n");
}

void create_project(char *project_name) 
{
  printf("Creating project %s\n", project_name);
  char *command = strcat("mkdir -p " AGOGO_DIR "/projects/", project_name);
  int status = system(command);

  if (status != 0) {
    printf("Error: Could not create the project directory for %s.\n", project_name);
    exit(EXIT_FAILURE);
  }
  free(command);
}

void destroy_project(char *project_name) 
{
  printf("Destroying project %s\n", project_name);
  char *command = strcat("rm -rf " AGOGO_DIR "/projects/", project_name);
  int status = system(command);

  if (status != 0) {
    printf("Error: Could not destroy the project directory for %s.\n", project_name);
    exit(EXIT_FAILURE);
  }
  free(command);
}
