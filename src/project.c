#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agogo.h"
#include "clock.h"
#include "project.h"


void list_projects();
void create_project(char *project_name);
void destroy_project(char *project_name);


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
  int status = system("ls " AGOGO_PROJECTS_DIR " | sort | column");

  if (status != 0) {
    printf("Error: Could not list the projects.\n");
    exit(EXIT_FAILURE);
  }
}


void create_project(char *project_name) 
{
  if (project_exists(project_name)) {
    printf("Error: Project %s already exists.\n", project_name);
    exit(EXIT_FAILURE);
  }

  char command[256];
  snprintf(command, sizeof(command), "mkdir -p " AGOGO_PROJECTS_DIR "/%s", project_name);

  int status = system(command);
  if (status != 0) {
    printf("Error: Could not create the project directory for %s.\n", project_name);
    exit(EXIT_FAILURE);
  }
  printf("Created project %s\n", project_name);
}


void destroy_project(char *project_name) 
{
  if (!project_exists(project_name)) {
    printf("Error: Project %s does not exist.\n", project_name);
    exit(EXIT_FAILURE);
  }

  if (is_clocked_on() == 0) {
    agogo_clockoff();
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


// reads the basename of the softlink AGOGO_CURRENTS_DIR/project
char *get_current_project() 
{
  char *command = "basename $(readlink " AGOGO_CURRENTS_DIR "/project )";
  FILE *fp = popen(command, "r");
  if (fp == NULL) {
    printf("Error: Could not read the current project.\n");
    exit(EXIT_FAILURE);
  }

  char project_name[256];
  fgets(project_name, sizeof(project_name), fp);
  pclose(fp);

  // remove the newline character
  project_name[strlen(project_name) - 1] = '\0';

  return strdup(project_name);
}


bool project_exists(char *project_name) 
{
  char project_path[256];
  snprintf(project_path, sizeof(project_path), AGOGO_PROJECTS_DIR "/%s", project_name);

  char command[512];
  snprintf(command, sizeof(command), "test -d %s", project_path);
  return (system(command) == 0);
}
