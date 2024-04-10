#ifndef LOG_H
#define LOG_H

#include "agogo.h"
#include "task.h"
#include "clock.h"

int agogo_log(int argc, char *argv[]);
void log_time(char *task_name, char *time_spent);
void parse_log_line(char* line, int *total_minutes, int *minutes);
char *get_log_file(char *task_name);
char *get_log_line(char *task_name, int line_number);

#endif // LOG_H
