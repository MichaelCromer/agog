#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agogo.h"
#include "task.h"
#include "clock.h"

void add_task(char *task_name);
void remove_task(char *task_name);

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

