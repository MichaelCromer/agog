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
void agogo_help();
void agogo_status();

// Project commands
int agogo_project(int argc, char *argv[]);
void list_projects();
void create_project(char *project_name);
void destroy_project(char *project_name);
char *get_current_project();
int project_exists(char *project_name);

// Task commands
int agogo_task(int argc, char *argv[]);
void list_tasks();
void add_task(char *task_name);
void remove_task(char *task_name);

// Clock commands
int agogo_clockon(int argc, char *argv[]);
int agogo_clockoff();
int is_clocked_on();

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
  else if (strcmp(first_command, "task") == 0) {
    exit_code = agogo_task(argc, argv);
  }
  else if (strcmp(first_command, "clockon") == 0) {
    exit_code = agogo_clockon(argc, argv);
  }
  else if (strcmp(first_command, "clockoff") == 0) {
    exit_code = agogo_clockoff();
  }
  else if (strcmp(first_command, "status") == 0) {
    agogo_status();
  }
  else if (strcmp(first_command, "help") == 0) {
    agogo_help();
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

void agogo_setup() {
  // create the agogo directory
  int status = system("mkdir -p " AGOGO_PROJECTS_DIR);
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
  return;
}

void agogo_status() {
  if (is_clocked_on() != 0) {
    printf("Not currently clocked on to any project.\n");
    return;
  }

  char *current_project = get_current_project();
  printf("Currently clocked on to project %s\n", current_project);
  printf("Tasks:\n");
  list_tasks();
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
  int status = system("ls " AGOGO_PROJECTS_DIR);

  if (status != 0) {
    printf("Error: Could not list the projects.\n");
    exit(EXIT_FAILURE);
  }
}

void create_project(char *project_name) 
{
  printf("Creating project %s\n", project_name);
  char command[256];
  snprintf(command, sizeof(command), "mkdir -p " AGOGO_PROJECTS_DIR "/%s", project_name);

  int status = system(command);
  if (status != 0) {
    printf("Error: Could not create the project directory for %s.\n", project_name);
    exit(EXIT_FAILURE);
  }
}

void destroy_project(char *project_name) 
{
  if (is_clocked_on() == 0) {
      char *current_project = get_current_project();
      if (strcmp(current_project, project_name) == 0) {
        printf("Error: Cannot destroy the project you are currently clocked on to.\n");
        exit(EXIT_FAILURE);
      }
  }

  if (project_exists(project_name) != 0) {
    printf("Error: Project %s does not exist.\n", project_name);
    exit(EXIT_FAILURE);
  }

  printf("Destroying project %s\n", project_name);
  char command[256];
  snprintf(command, sizeof(command), "rm -rf " AGOGO_PROJECTS_DIR "/%s", project_name);

  int status = system(command);
  if (status != 0) {
    printf("Error: Could not destroy the project directory for %s.\n", project_name);
    exit(EXIT_FAILURE);
  }
}

char *get_current_project() 
{
  char *current_project = NULL;
  FILE *fp;
  char path[1035];

  fp = popen("readlink " AGOGO_DIR "/current", "r");
  if (fp == NULL) {
    printf("Error: Could not read the current project.\n");
    exit(EXIT_FAILURE);
  }
  
  while (fgets(path, sizeof(path)-1, fp) != NULL) {
    current_project = path;
  }
  
  pclose(fp);

  if (current_project == NULL) {
    printf("Error: Could not read the current project.\n");
    exit(EXIT_FAILURE);
  }

  char *last_slash = strrchr(current_project, '/');
  current_project = last_slash + 1;

  return current_project;
}

int project_exists(char *project_name) 
{
  char project_path[256];
  snprintf(project_path, sizeof(project_path), "%s/%s", AGOGO_PROJECTS_DIR, project_name);

  char command[512];
  snprintf(command, sizeof(command), "test -d %s", project_path);
  int status = system(command);
  return status;
}

/* ============================================================
 *  TASK
 * ============================================================
 */

int agogo_task(int argc, char *argv[])
{
  if (argc < 3) {
    list_tasks();
    return EXIT_SUCCESS;
  }

  char *sub_command = argv[2];

  if ((strcmp(sub_command, "--add") == 0) || (strcmp(sub_command, "-a") == 0)) {
    if (argc < 4) {
      printf("Error: Missing task name\n");
      return EXIT_FAILURE;
    }
    add_task(argv[3]);
  }
  if ((strcmp(sub_command, "--remove") == 0) || (strcmp(sub_command, "-r") == 0)) {
    if (argc < 4) {
      printf("Error: Missing task name\n");
      return EXIT_FAILURE;
    }
    remove_task(argv[3]);
  }

  return EXIT_SUCCESS;
}

void list_tasks() 
{
  if (is_clocked_on() != 0) {
    printf("Error: Not currently clocked on to any project.\n");
    return;
  }

  int status = system("ls " AGOGO_DIR "/current");

  if (status != 0) {
    printf("Error: Could not list the tasks.\n");
    exit(EXIT_FAILURE);
  }
}

void add_task(char *task_name) 
{
  if (is_clocked_on() != 0) {
    printf("Error: Not currently clocked on to any project.\n");
    return;
  }
  printf("Adding task %s\n", task_name);

  char command[256];
  snprintf(command, sizeof(command), "touch " AGOGO_DIR "/current/%s", task_name);

  int status = system(command);
  if (status != 0) {
    printf("Error: Could not add the task %s.\n", task_name);
    exit(EXIT_FAILURE);
  }
}

void remove_task(char *task_name) 
{
  if (is_clocked_on() != 0) {
    printf("Error: Not currently clocked on to any project.\n");
    return;
  }
  printf("Removing task %s\n", task_name);
  char command[256];
  snprintf(command, sizeof(command), "rm " AGOGO_DIR "/current/%s", task_name);

  int status = system(command);
  if (status != 0) {
    printf("Error: Could not remove the task %s.\n", task_name);
    exit(EXIT_FAILURE);
  }
}



/* ============================================================
 *  CLOCK
 * ============================================================
 */

int agogo_clockon(int argc, char *argv[]) 
{
  if (argc < 3) {
    printf("Error: Missing project name\n");
    return EXIT_FAILURE;
  }

  if (strcmp(argv[2], "-p") == 0) {
    if (argc < 4) {
      printf("Error: Missing project name\n");
      return EXIT_FAILURE;
    }

  }
  char *project_name = argv[2];

  // first check if the project exists
  char project_path[256];
  snprintf(project_path, sizeof(project_path), "%s/%s", AGOGO_PROJECTS_DIR, project_name);

  char command[512];
  snprintf(command, sizeof(command), "test -d %s", project_path);
  int status = system(command);
  if (status != 0) {
    printf("Error: Project %s does not exist.\n", project_name);
    exit(EXIT_FAILURE);
  }

  // then check if we are already clocked on to a project
  if (is_clocked_on() == 0) {
    system("rm " AGOGO_DIR "/current");
  }

  printf("Clocking on to project %s\n", project_name);
  snprintf(command, sizeof(command), "ln -s %s %s/current", project_path, AGOGO_DIR);

  status = system(command);
  if (status != 0) {
    printf("Error: Could not clock on to project %s.\n", project_name);
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}

int agogo_clockoff() 
{
  if (is_clocked_on() != 0) 
  {
    printf("Error: Not currently clocked on to any project.\n");
    return EXIT_FAILURE;
  }

  printf("Clocking off\n");
  int status = system("rm " AGOGO_DIR "/current");
  if (status != 0) {
    printf("Error: Could not clock off.\n");
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}

int is_clocked_on() 
{
  int status = system("test -L " AGOGO_DIR "/current");
  return status;
}
