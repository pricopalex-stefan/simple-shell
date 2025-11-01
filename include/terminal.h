#ifndef _TERMINAL__H
#define _TERMINAL__H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <termios.h>
#include "../include/history.h"

#define MAX_INPUT 1024
#define MAX_ARGS 32

void disable_raw_mode();
void enable_raw_mode();
void read_input(char* buffer);

#endif
