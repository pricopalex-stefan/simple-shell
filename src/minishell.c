#include "../include/minishell.h"

/**
 * A simple shell implementation.
 */

static char history[MAX_HISTORY][MAX_COMMAND_LENGTH];
static int history_count = 0;
static int current_index = 0;

int main() {
    char input[MAX_INPUT];
    char* args[MAX_ARGS];

    while (1) {
        printf("msh> ");
        fflush(stdout);

        read_input(input, history, &history_count, &current_index);

	if(strlen(input) == 0) continue;
	
	// add input to command history
	add_to_history(input, history, &history_count, &current_index);

        // check for exit command
        if (strcmp(input, "exit") == 0) {
            printf("mshell: bye\n");
            break;
        }
	
	// check for print history command
	if (strcmp(input, "print_h") == 0) {
    	    print_history(history, &history_count, &current_index);
	    continue;
    	}

        // tokenize input
        char* token = strtok(input, " ");
        int i = 0;

        while (token != NULL && i < MAX_ARGS) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;

        // change directory command
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "Missing argument for cd\n");
            } else if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
            continue;
        }

        // fork a child process
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            continue;
        }

        // child process
        if (pid == 0) {
            int status_code = execvp(args[0], args);
            if (status_code == -1) {
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        } 
        // parent process
        else {
            int pid_status;
            waitpid(pid, &pid_status, 0);
            printf("Exit status: %d\n", WEXITSTATUS(pid_status)); 
        }
    }
    return 0;
}

