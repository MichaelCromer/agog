#include "log.h"


int agogo_log(int argc, char **argv)
{
  if (argc < 3) {
    printf("Not implemented yet\n");
    return EXIT_SUCCESS;
  }

  if ((strcmp(argv[2], "--time") == 0) | (strcmp(argv[2], "-t") == 0)) {
    if (argc < 4) {
      printf("Error: Not enough arguments.\n");
      return EXIT_FAILURE;
    }
    log_time(argv[3]);
  }

  return EXIT_SUCCESS;
}


void log_time(char *time_spent)
{
  if (!is_clocked_on()) {
    printf("Error: You are not clocked on.\n");
    exit(EXIT_FAILURE);
  }

  if (!has_current_task()) {
    printf("Error: You do not have a current task.\n");
    exit(EXIT_FAILURE);
  }

  char *task_name = get_current_task();
  char command[256];
  int num_minutes = parse_time(time_spent);

  snprintf(command, sizeof(command), "echo %d >> " AGOGO_DIR "/current/%s", num_minutes, task_name);

  int status = system(command);
  if (status != 0) {
    printf("Error: Could not log the time %s for task %s.\n", time_spent, task_name);
    exit(EXIT_FAILURE);
  }
  printf("Logging %d minutes for task %s\n", num_minutes, task_name);
}

void read_log_line(char *line, char *timestamp, int *total_minutes, int *minutes)
{
  sscanf(line, "%s %d %d", timestamp, total_minutes, minutes);
}
