#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 32

/**
 * A simple shell implementation.
 */
int main() {
    char input[MAX_INPUT];
    char* args[MAX_ARGS];

    while (1) {
        printf("msh> ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            perror("fgets failed");
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        // check for exit command
        if (strcmp(input, "exit") == 0) {
            printf("mshell: bye\n");
            break;
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

