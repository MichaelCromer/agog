#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "agog.h"
#include "clock.h"

int agog_project(int argc, char *argv[]);
char *get_current_project();
bool project_exists(char *project);
void list_projects();
void create_project(char *project_name);
void destroy_project(char *project_name);

#endif // PROJECT_H
