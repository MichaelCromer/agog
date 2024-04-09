#ifndef PROJECT_H
#define PROJECT_H

#include <stdbool.h>

int agogo_project(int argc, char *argv[]);
char *get_current_project();
bool project_exists(char *project);

#endif // PROJECT_H
