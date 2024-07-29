#ifndef AGOG_CLOCK_H
#define AGOG_CLOCK_H

#include <ctype.h>
#include <stdbool.h>

#include "agog.h"
#include "project.h"

int agog_clockon(int argc, char *argv[]);
int agog_clockoff();
bool is_clocked_on();
int parse_time(char *time_str);

#endif // AGOG_CLOCK_H
