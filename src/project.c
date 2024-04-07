#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agogo.h"
#include "clock.h"

void list_projects();
void create_project(char *project_name);
void destroy_project(char *project_name);
int project_exists(char *project_name);


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
  if (project_exists(project_name) == 0) {
    printf("Error: Project %s already exists.\n", project_name);
    exit(EXIT_FAILURE);
  }

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
