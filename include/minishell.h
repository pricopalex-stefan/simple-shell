#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

#define MAX_INPUT 1024
#define MAX_ARGS 32
#define MAX_HISTORY 100
#define MAX_COMMAND_LENGTH 1024

// default terminal configuration
extern struct termios default_t;

void add_to_history(char const *command, char history[][MAX_COMMAND_LENGTH], int *history_count, int *current_index);
const char* get_history_up(char history[][MAX_COMMAND_LENGTH], int *history_count, int *current_index);
const char* get_history_down(char history[][MAX_COMMAND_LENGTH], int *history_count, int *current_index);
void print_history(char history[][MAX_COMMAND_LENGTH], int *history_count, int *current_index);
void disable_raw_mode();
void enable_raw_mode();
void read_input(char* buffer, char history[][MAX_COMMAND_LENGTH], int *history_count, int *current_index);

#endif
