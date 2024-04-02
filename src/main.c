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
#define AGOGO_DIR "$HOME/.agogo"
#endif

/* ------------------------------------------------------------
 * PROTOTYPES
 * */

// Help and print
void print_short_help();

// Primary commands
int agogo_project(int argc, char *argv[]);

// Project commands
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
  if (strcmp(first_command, "project") == 0) {
    exit_code = agogo_project(argc, argv);
  }

  return exit_code;
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

  if (strcmp(sub_command, "--create") == 0) {
    if (argc < 4) {
      printf("Error: Missing project name\n");
      return EXIT_FAILURE;
    }
    create_project(argv[3]);
  }

  if (strcmp(sub_command, "--destroy") == 0) {
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
}

void destroy_project(char *project_name) 
{
  printf("Destroying project %s\n", project_name);
}
