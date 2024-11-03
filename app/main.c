#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define HISTORY_SIZE 10
#define HISTORY_FILE "history.txt"

void sighup_handler(int sig) {
    printf("Configuration reloaded\n");
}

void handle_echo(char *input) {
    input[strcspn(input, "\n")] = 0;

    if (strcmp(input, "echo") == 0) {
        return;
    } 
    else if (strcmp(input, "echo ") == 0) {
        printf("\n");
    } 
    else if (strncmp(input, "echo ", 5) == 0) {
        char *arg = input + 5;
        if ((arg[0] == '"' && arg[strlen(arg) - 1] == '"') ||
            (arg[0] == '\'' && arg[strlen(arg) - 1] == '\'')) {
            arg[strlen(arg) - 1] = '\0';
            arg++;
        }
    printf("%s\n", arg);
    } else {
        printf("Incorrect using echo command!\n");
    }
}

void execute_command(char *command) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        char *argv[64];  // Предполагаем, что не будет больше 64 аргументов 
        char *token = strtok(command, " ");
        int i = 0;

        // Заполняем массив argv 
        while (token != NULL) {
            argv[i++] = token;
            token = strtok(NULL, " ");
        }
        argv[i] = NULL; // Завершаем массив NULL

        // Формируем полный путь к исполняемому файлу 
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "/bin/%s", argv[0]);

        execv(full_path, argv); // Выполняем бинарный файл 
        perror("execv failed");
        exit(EXIT_FAILURE);
    } 
    
    else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    } 
    
    else {
        perror("fork failed");
    }
}

int main() {
    signal(SIGHUP, sighup_handler);

    char input[1024];

    char* history[HISTORY_SIZE];
    for (int i = 0; i < HISTORY_SIZE; i++) {
        history[i] = malloc(1024);
    }
    int history_index = 0;
    FILE* history_file = fopen(HISTORY_FILE, "a+");

    char line[1024];
    while (fgets(line, 1024, history_file) != NULL) {
        strcpy(history[history_index], line);
        history_index = (history_index + 1) % HISTORY_SIZE;
    }

    while (fgets(input, 1024, stdin) != NULL) {

        strcpy(history[history_index], input);
        history_index = (history_index + 1) % HISTORY_SIZE;
        fprintf(history_file, "%s", input);

        if (strcmp(input, "exit\n") == 0 || strcmp(input, "\\q\n") == 0) {
            break;
        } 
        
        else if (strncmp(input, "echo", 4) == 0) {
            handle_echo(input);
        } 
        
        else if (strncmp(input, "\\e ", 3) == 0) {
            char* var_name = input + 4;
            var_name[strcspn(var_name, "\n")] = 0;
            char* var_value = getenv(var_name);
            if (var_value != NULL) 
                printf("%s\n", var_value);
            else
                printf("Variable not found: %s\n", var_name);
        } 
        
        else if (strncmp(input, "\\l", 2) == 0) {
            char* device = input + 3;
            device[strcspn(device, "\n")] = 0;
            char cmd[1024];
            snprintf(cmd, sizeof(cmd), "lsblk %s", device);
            execute_command(cmd);
        } 
        
        else if (strncmp(input, "run ", 4) == 0) {
            // Выполняем команду, начинающуюся с "run"
            char* command_to_run = input + 4;
            command_to_run[strcspn(command_to_run, "\n")] = 0;
            execute_command(command_to_run);
        } 
        
        else {
            printf("The command isn't found!\n");
        }
    }

    fclose(history_file);
    return 0;
}
