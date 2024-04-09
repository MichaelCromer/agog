#ifndef AGOGO_CLOCK_H
#define AGOGO_CLOCK_H

#include <stdbool.h>

int agogo_clockon(int argc, char *argv[]);
int agogo_clockoff();
bool is_clocked_on();
int parse_time(char *time_str);

#endif // AGOGO_CLOCK_H
