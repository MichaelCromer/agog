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
    if (!has_current_task()) {
      printf("Error: You have not set any active task.\n");
      return EXIT_FAILURE;
    }
    char *task_name = get_current_task();
    log_time(task_name, argv[3]);
  }

  return EXIT_SUCCESS;
}


void log_time(char *task_name, char *time_spent)
{
  if (!is_clocked_on()) {
    printf("Error: You are not clocked on to any project.\n");
    exit(EXIT_FAILURE);
  }

  int minutes = parse_time(time_spent);
  int old_minutes = 0;
  int old_total = 0;

  char *log_file = get_log_file(task_name);
  char *line = get_log_line(log_file, 0);
  if (line != NULL) {
    parse_log_line(line, &old_total, &old_minutes);
  }
  int new_total = old_total + minutes;

  char command[256];
  snprintf(command, sizeof(command), "echo \"%d %d\" >> %s", new_total, minutes, log_file);
  int status = system(command);
  if (status != 0) {
    printf("Error: Could not log the time %s for task %s.\n", time_spent, task_name);
    exit(EXIT_FAILURE);
  }
  printf("Logging %d minutes for task %s\n", minutes, task_name);
}

void parse_log_line(char* line, int *total_minutes, int *minutes)
{
  sscanf(line, "%d %d", total_minutes, minutes);
}

char *get_log_line(char *log_file, int line_number) 
{
  FILE *file = fopen(log_file, "r");
  if (file == NULL) {
    printf("Error: Could not open log file %s.\n", log_file);
    exit(EXIT_FAILURE);
  }

  char *line = NULL;
  size_t len = 0;
  int current_line = 0;
  while (getline(&line, &len, file) != -1) {
    if (current_line == line_number) {
      break;
    }
    current_line++;
  }

  fclose(file);
  return line;
}
  

//probably move this to task.c
char *get_log_file(char *task_name)
{
  char *log_file = malloc(256);
  snprintf(log_file, 256, AGOGO_CURRENTS_DIR "/project/%s", task_name);
  return log_file;
}
