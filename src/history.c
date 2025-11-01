#include "../include/history.h"

static struct History *cmd_history;

void init_history() {
	cmd_history = (struct History*)malloc(sizeof(struct History));
	cmd_history->history_count = 0;
	cmd_history->current_index = 0;
}

// adding a command to the command history
void add_to_history(char* command) {
        if(cmd_history->history_count < MAX_HISTORY) {
                if (strlen(command) < MAX_COMMAND_LENGTH) {
                        strncpy(cmd_history->history[cmd_history->history_count], command, MAX_COMMAND_LENGTH - 1);
                        cmd_history->history[cmd_history->history_count][MAX_COMMAND_LENGTH - 1] = '\0';
                        cmd_history->history_count++;
                }
                cmd_history->current_index = cmd_history->history_count;
        }
}

// by pressing the UP key this fuction will return the previous command if it exists
// returns an empty string if at the start of history
const char* get_history_up() {
        if(cmd_history->current_index > 0) {
                cmd_history->current_index--;
                return cmd_history->history[cmd_history->current_index];
        }
        return NULL;
}

// by pressing DOWN key this function will return the next command from the history if it exists
// returns an empty stirng if at the end of history and resets the current_index to history_count
const char* get_history_down() {
        if(cmd_history->current_index < (cmd_history->history_count) - 1) {
                (cmd_history->current_index)++;
                return cmd_history->history[cmd_history->current_index];
        } else {
                cmd_history->current_index = cmd_history->history_count;
                return "";
        }

        return NULL;
}

void print_history(){
        int i;
        for(i = 0; i < cmd_history->history_count; i++) {
                printf("%s\n", cmd_history->history[i]);
        }
}

void reset_current_index() {
	cmd_history->current_index = cmd_history->history_count;
}

