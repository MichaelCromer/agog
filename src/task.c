#include "task.h"


int agog_task(int argc, char *argv[])
{
  if (argc < 3) {
    list_tasks();
    return EXIT_SUCCESS;
  }

  char *sub_command = argv[2];

  // Create a new task; arg is the task name
  if ((strcmp(sub_command, "--add") == 0) || (strcmp(sub_command, "-a") == 0)) {
    if (argc < 4) {
      printf("Error: Missing task name\n");
      return EXIT_FAILURE;
    }
    add_task(argv[3]);
  }

  // Remove the task; arg is the task name
  else if ((strcmp(sub_command, "--remove") == 0) || (strcmp(sub_command, "-r") == 0)) {
    if (argc < 4) {
      printf("Error: Missing task name\n");
      return EXIT_FAILURE;
    }
    remove_task(argv[3]);
  }

  // Set the given task as the current task; arg is the task name
  else if ((strcmp(sub_command, "--set") == 0) || (strcmp(sub_command, "-s") == 0)) {
    if (argc < 4) {
      printf("Error: Missing task name\n");
      return EXIT_FAILURE;
    }
    set_current_task(argv[3]);
  }

  // Unset the current task if any 
  else if ((strcmp(sub_command, "--unset") == 0) || (strcmp(sub_command, "-u") == 0)) {
    printf("Not implemented yet\n");
  }

  // Get a random new task
  else if ((strcmp(sub_command, "--get") == 0) || (strcmp(sub_command, "-g") == 0)) {
    printf("Not implemented yet\n");
  }

  // Move the task to a new name; args are the old task name and new task name
  else if ((strcmp(sub_command, "--move") == 0) || (strcmp(sub_command, "-m") == 0)) {
    if (argc < 5) {
      printf("Error: Missing task name\n");
      return EXIT_FAILURE;
    }
    move_task(argv[3], argv[4]);
  }

  // Copy the task; args are the task name and project name
  else if ((strcmp(sub_command, "--copy") == 0) || (strcmp(sub_command, "-c") == 0)) {
    printf("Not implemented yet\n");
  }

  // Set the priority of the task; args are the task name and priority level
  else if ((strcmp(sub_command, "--priority") == 0) || (strcmp(sub_command, "-p") == 0)) {
    printf("Not implemented yet\n");
  }

  // Mark the task as done; arg is the task name
  else if ((strcmp(sub_command, "--done") == 0) || (strcmp(sub_command, "-d") == 0)) {
    printf("Not implemented yet\n");
  }

  else {
    printf("Error: Unknown subcommand %s for agog-task\n", sub_command);
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}


void list_tasks() 
{
  if (!is_clocked_on()) {
    printf("Error: Not currently clocked on to any project.\n");
    return;
  }

  int status = system("ls " AGOG_CURRENTS_DIR "/project | sort | column");

  if (status != 0) {
    printf("Error: Could not list the tasks.\n");
    exit(EXIT_FAILURE);
  }
}


void add_task(char *task_name) 
{
  if (!is_clocked_on()) {
    printf("Error: Not currently clocked on to any project.\n");
    return;
  }

  if (task_exists(task_name)) {
    printf("Error: Task %s already exists.\n", task_name);
    exit(EXIT_FAILURE);
  }

  char command[256];
  snprintf(command, sizeof(command), "echo \"0 0\" > " AGOG_CURRENTS_DIR "/project/%s", task_name);
  if (system(command) != 0) {
    printf("Error: Could not add the task %s.\n", task_name);
    exit(EXIT_FAILURE);
  }

  printf("Added task %s\n", task_name);
}


void remove_task(char *task_name) 
{
  if (!is_clocked_on()) {
    printf("Error: Not currently clocked on to any project.\n");
    return;
  }
  char command[256];
  snprintf(command, sizeof(command), "rm " AGOG_CURRENTS_DIR "/project/%s", task_name);

  int status = system(command);
  if (status != 0) {
    printf("Error: Could not remove the task %s.\n", task_name);
    exit(EXIT_FAILURE);
  }
  printf("Removed task %s\n", task_name);
}


// TODO this will eventually work for moving tasks between projects
void move_task(char *old_task, char *new_task) {
  if (!task_exists(old_task)) {
    printf("Error: Task %s does not exist.\n", old_task);
    exit(EXIT_FAILURE);
  }

  char command[256];
  snprintf(command, sizeof(command), "mv " AGOG_CURRENTS_DIR "/project/%s " AGOG_CURRENTS_DIR "/project/%s", old_task, new_task);

  int status = system(command);
  if (status != 0) {
    printf("Error: Could not move the task %s to %s.\n", old_task, new_task);
    exit(EXIT_FAILURE);
  }

  printf("Renamed task %s to %s\n", old_task, new_task);
}


void set_current_task(char *task_name) 
{
  if (!task_exists(task_name)) {
    printf("Error: Task %s does not exist.\n", task_name);
    exit(EXIT_FAILURE);
  }

  if (has_current_task()) {
    int status = system("rm " AGOG_DIR "/current/task");
    if (status != 0) {
      printf("Error: Could not unset the current task.\n");
      exit(EXIT_FAILURE);
    }
  }

  char command[256];
  snprintf(command, sizeof(command), "ln -s " AGOG_CURRENTS_DIR "/project/%s " AGOG_CURRENTS_DIR "/task", task_name);

  int status = system(command);
  if (status != 0) {
    printf("Error: Could not set the current task to %s.\n", task_name);
    exit(EXIT_FAILURE);
  }

  printf("Set the current task to %s\n", task_name);
}


char *get_current_task() 
{
  char *command = "basename $(readlink " AGOG_CURRENTS_DIR "/task )";
  FILE *fp = popen(command, "r");
  if (fp == NULL) {
    printf("Error: Could not read the current task.\n");
    exit(EXIT_FAILURE);
  }

  char task_name[256];
  fgets(task_name, sizeof(task_name), fp);
  pclose(fp);

  // remove the newline character
  task_name[strlen(task_name) - 1] = '\0';

  return strdup(task_name);
}


bool has_current_task() 
{
  if (!is_clocked_on()) {
    return false;
  }

  char *command = "test -f " AGOG_DIR "/current/task";
  int status = system(command);
  if (status != 0) {
    return false;
  }
  return true;
}


// TODO eventually this will accept a project name too
bool task_exists(char *task_name)
{
  if (!is_clocked_on()) {
    return false;
  }

  char command[256];
  snprintf(command, sizeof(command), "test -f " AGOG_CURRENTS_DIR "/project/%s", task_name);

  int status = system(command);
  if (status != 0) {
    return false;
  }
  return true;
}
