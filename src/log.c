#include "log.h"


void log_time(char *task_name, char *time_spent);

int agogo_log(int argc, char **argv)
{
  if (argc != 4) {
    printf("Usage: agogo log <task_name> <time_spent>\n");
    return EXIT_FAILURE;
  }

  char *task_name = argv[2];
  char *time_spent = argv[3];

  log_time(task_name, time_spent);

  return EXIT_SUCCESS;
}


void log_time(char *task_name, char *time_spent)
{
  if (task_exists(task_name) != 0) {
    printf("Error: Task %s does not exist.\n", task_name);
    exit(EXIT_FAILURE);
  }

  char command[256];
  int num_minutes = parse_time(time_spent);

  printf("Logging %d minutes for task %s\n", num_minutes, task_name);
  snprintf(command, sizeof(command), "echo %d >> " AGOGO_DIR "/current/%s", num_minutes, task_name);

  int status = system(command);
  if (status != 0) {
    printf("Error: Could not log the time %s for task %s.\n", time_spent, task_name);
    exit(EXIT_FAILURE);
  }

}
