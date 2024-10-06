#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HISTORY_SIZE 10
#define HISTORY_FILE "history.txt"

int main() {
  // Для ввода
  char input[1024];

  // Для истории команд
  char* history[HISTORY_SIZE];
  for (int i = 0; i < HISTORY_SIZE; i++) 
  {
        history[i] = malloc(1024);
  }
  int history_index = 0;
  FILE* history_file = fopen(HISTORY_FILE, "a+");

  char line[1024];
  while (fgets(line, 1024, history_file) != NULL) 
  {
      strcpy(history[history_index], line);
      history_index = (history_index + 1) % HISTORY_SIZE;
  }

  while (fgets(input, 1024, stdin) != NULL) 
  {
    // Сохранение команды в историю
    strcpy(history[history_index], input);
    history_index = history_index+1;
    if (history_index == 10)
        history_index = 0;
    fprintf(history_file, "%s", input);

    // Команды exit и \q
    if (strcmp(input, "exit\n") == 0 || strcmp(input, "\\q\n") == 0)
    {
        break;
    }

    // Команда history
    else if (strcmp(input, "history\n") == 0)
    {
        for (int i = 0; i < HISTORY_SIZE; i++) 
        {
            printf("%d: %s", i + 1, history[i]);
        }
    }

    // Команда echo
    else if (strncmp(input, "echo", 4) == 0)
    {
        printf("%s", input + 5);
    }

    // Команда \e <variable>
    else if (strncmp(input, "\\e ", 3) == 0) 
    {
        char* var_name = input + 4;
        var_name[strcspn(var_name, "\n")] = 0;
        char* var_value = getenv(var_name);
        if (var_value != NULL) 
            printf("%s\n", var_value);
        else
            printf("Variable not found: %s\n", var_name);
    }

    // Выполнить указанный бинарник
    else
    {
        input[strcspn(input, "\n")] = 0;
        system(input);
    }
  }
  fclose(history_file);
  return 0;
}

