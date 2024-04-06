#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agogo.h"
#include "clock.h"

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
