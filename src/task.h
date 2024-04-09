#ifndef AGOGO_TASK_H
#define AGOGO_TASK_H

#include <stdbool.h>

int agogo_task(int argc, char *argv[]);
void list_tasks();
bool task_exists(char *task_name);

#endif // AGOGO_TASK_H
