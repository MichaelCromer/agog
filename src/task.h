#ifndef AGOGO_TASK_H
#define AGOGO_TASK_H

#include <stdbool.h>

#include "agogo.h"
#include "clock.h"

int agogo_task(int argc, char *argv[]);
void list_tasks();
void add_task(char *task_name);
void remove_task(char *task_name);
void move_task(char *old_task, char *new_task);
void set_current_task(char *task_name);
char *get_current_task();
bool task_exists(char *task_name);
bool has_current_task();

#endif // AGOGO_TASK_H
