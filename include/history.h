#ifndef _HISTORY_H
#define _HISTORY_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_HISTORY 100
#define MAX_COMMAND_LENGTH 1024

struct History {
	char history[MAX_HISTORY][MAX_COMMAND_LENGTH];
	int history_count;
	int current_index;
};

void init_history();
void add_to_history(char* command);
const char* get_history_up();
const char* get_history_down();
void print_history();
void reset_current_index();

#endif
