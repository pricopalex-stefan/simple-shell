#include "../include/terminal.h"

//default configuration of terminal
static struct termios default_t;

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
void read_input(char* buffer) {
	int len = 0;
	buffer[len] = '\n';
	
	// reset current command history index to history count after pressing enter
	reset_current_index();
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
							const char* prevCmd = get_history_up();

							if(prevCmd) {
								for(int i = 0; i < len; i++) printf("\b \b");
								len = snprintf(buffer, MAX_INPUT, "%s", prevCmd);
								printf("%s", buffer);
								fflush(stdout);
							} 

					} else if(seq[1] == 'B') {
						
							// deleting the arrow_down in the terminal before printing the next
							// command from history
							const char* nextCmd = get_history_down();
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






