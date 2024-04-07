#ifndef AGOGO_CLOCK_H
#define AGOGO_CLOCK_H

int agogo_clockon(int argc, char *argv[]);
int agogo_clockoff();
int is_clocked_on();
int parse_time(char *time_str);

#endif // AGOGO_CLOCK_H
