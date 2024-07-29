#include "status.h"


int agog_status() {
  char *current_project = NULL;
  char *current_task = NULL;

  if (is_clocked_on()) {
    current_project = get_current_project();
  } else {
    current_project = "";
  }

  if (has_current_task()) {
    current_task = get_current_task();
  } else {
    current_task = "";
  }

  printf("Project: %s\n", current_project);
  printf("Task: %s\n", current_task);

  return EXIT_SUCCESS;
}
