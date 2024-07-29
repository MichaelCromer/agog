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

#include "agog.h"
#include "project.h"
#include "task.h"
#include "clock.h"
#include "log.h"
#include "status.h"

/* ------------------------------------------------------------
 * PROTOTYPES
 * */

// Setup
void agog_setup();
void check_setup();

// Help and print
void print_short_help();
void agog_help();


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
    agog_setup();
  }
  check_setup();

  if (strcmp(first_command, "project") == 0) {
    exit_code = agog_project(argc, argv);
  }
  else if (strcmp(first_command, "task") == 0) {
    exit_code = agog_task(argc, argv);
  }
  else if (strcmp(first_command, "clockon") == 0) {
    exit_code = agog_clockon(argc, argv);
  }
  else if (strcmp(first_command, "clockoff") == 0) {
    exit_code = agog_clockoff();
  }
  else if (strcmp(first_command, "log") == 0) {
    exit_code = agog_log(argc, argv);
  }
  else if (strcmp(first_command, "status") == 0) {
    exit_code = agog_status();
  }
  else if (strcmp(first_command, "help") == 0) {
    agog_help();
  }
  else {
    printf("Error: Unknown command %s\n", first_command);
    exit_code = EXIT_FAILURE;
  }

  return exit_code;
}

/* ============================================================
 *  SETUP
 * ============================================================
 */


void agog_setup() {
  // create the agog directory
  int status = system("mkdir -p " AGOG_PROJECTS_DIR);
  if (status != 0) {
    printf("Error: Could not create the agog directory.\n");
    exit(EXIT_FAILURE);
  }
}


void check_setup() {
  // check that the agog directory exists
  int status = system("test -d " AGOG_DIR);
  if (status != 0) {
    printf("Error: Agogo directory not found. Please run `agog setup` to create the directory.\n");
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
  printf("Version: %s\n", AGOG_VERSION);
  printf("============================================================\n");
}


void agog_help() {
  return;
}

