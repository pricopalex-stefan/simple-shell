#include "../include/minishell.h"

struct termios default_t;

// adding a command to the command history 
void add_to_history(char const *command, char history[][MAX_COMMAND_LENGTH], int *history_count, int *current_index) {
	if(*history_count < MAX_HISTORY) {
		if (strlen(command) < MAX_COMMAND_LENGTH) {
			strncpy(history[*history_count], command, MAX_COMMAND_LENGTH - 1);
			history[*history_count][MAX_COMMAND_LENGTH - 1] = '\0';
			(*history_count)++;
		}
		current_index = history_count;
	}
}

// by pressing the UP key this fuction will return the previous command if it exists
// returns an empty string if at the start of history
const char* get_history_up(char history[][MAX_COMMAND_LENGTH], int *history_count, int *current_index) {
	if(*current_index > 0) {
		(*current_index)--;
		return history[*current_index];
	}
	return NULL;
}

// by pressing DOWN key this function will return the next command from the history if it exists
// returns an empty stirng if at the end of history and resets the current_index to history_count
const char* get_history_down(char history[][MAX_COMMAND_LENGTH], int *history_count, int *current_index) {
	if(*current_index < (*history_count) - 1) {
		(*current_index)++;
		return history[*current_index];
	} else {
		*current_index = *history_count;
		return "";
	}

	return NULL;
}

void print_history(char history[][MAX_COMMAND_LENGTH], int *history_count, int *current_index){
	int i;
	for(i = 0; i < *history_count; i++) {
		printf("%s\n", history[i]);
	}
}

// restoring the terminal to default settings
void disable_raw_mode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &default_t);
}

// sets the terminal to raw mode by:
// storing the default terminal settings in the default_t
// disabling echo , so typed characters are not shown on the screen
// disabling canonical mode, allowing input to be read character by character
// setting the c_cc[VMIN] to > 0, meaning at least one byte is required for read() to return
// setting the c_cc[VTIME] to 0, meaning there will be no timeout for read()
void enable_raw_mode() {
	tcgetattr(STDIN_FILENO, &default_t);
	atexit(disable_raw_mode);
	
	struct termios raw = default_t;
	raw.c_lflag &= ~(ECHO | ICANON);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

}

// read input from user , enanbling raw terminal mode to read one character at the time
// allowing to navigate trough the command history via arrow_up and arrow_down
void read_input(char* buffer, char history[][MAX_COMMAND_LENGTH], int *history_count, int *current_index) {
	int len = 0;
	buffer[len] = '\n';
	
	// reset current command history index to history count after pressing enter
	*current_index = *history_count;
	enable_raw_mode();

	char c;

	while(read(STDIN_FILENO, &c, 1) == 1) {
		// enter handling
		if(c == '\n') {
			buffer[len] = '\0';
			printf("\n");
			break;
		// backspace handling
		} else if (c == '\b' || c == 127) {
			if(len > 0) {
				len--;
				buffer[len] = '\0';
				printf("\b \b");
				fflush(stdout);
			}
		// ESCAPE character witch is present before the arrow up/down sequence of characters	
		} else if(c == 27) {
			char seq[2];
			if(read(STDIN_FILENO, &seq[0], 1) == 1 && 
			   read(STDIN_FILENO, &seq[1], 1) == 1) {
				if(seq[0] == '[') {
					if(seq[1] == 'A') {

							// deleting the arrow_down in the terminal before printing the
							// previous command from history
							const char* prevCmd = get_history_up(history, history_count, current_index);

							if(prevCmd) {
								for(int i = 0; i < len; i++) printf("\b \b");
								len = snprintf(buffer, MAX_INPUT, "%s", prevCmd);
								printf("%s", buffer);
								fflush(stdout);
							} 

					} else if(seq[1] == 'B') {
						
							// deleting the arrow_down in the terminal before printing the next
							// command from history
							const char* nextCmd = get_history_down(history, history_count, current_index);
							for(int i = 0; i < len; i++) printf("\b \b");

							if(nextCmd) {
								len = snprintf(buffer, MAX_INPUT, "%s", nextCmd);
								printf("%s", buffer);
								fflush(stdout);
							} else {
								len = 0;
								buffer[len] = '\0';
								fflush(stdout);
							}
								
					}
				}
			}
		// writing each character at the time	
		} else if(len < MAX_INPUT - 1) {
			buffer[len++] = c;
			write(STDOUT_FILENO, &c, 1);
		}
	}

	disable_raw_mode();
}






