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
    // Команды exit и \q
    if (strcmp(input, "exit\n") == 0 || strcmp(input, "\\q\n") == 0)
    {
        strcpy(history[history_index], input);
        history_index = history_index+1;
        if (history_index == 10)
           history_index = 0;
        fprintf(history_file, "%s", input);
        break;
    }

    // Команда history
    else if (strcmp(input, "history\n") == 0)
    {
        strcpy(history[history_index], input);
        history_index = history_index+1;
        if (history_index == 10)
        history_index = 0;
        fprintf(history_file, "%s", input);
        for (int i = 0; i < HISTORY_SIZE; i++) 
        {
            printf("%d: %s", i + 1, history[i]);
        }
    }

    // Команда echo
    else if (strncmp(input, "echo", 4) == 0)
    {
        strcpy(history[history_index], input);
        history_index = history_index+1;
        if (history_index == 10)
            history_index = 0;
        fprintf(history_file, "%s", input);
        printf("%s", input + 5);
    }

    // Команда не найдена
    else
    {
      printf("The command is not found!\n");
      strcpy(history[history_index], input);
      history_index = history_index+1;
      if (history_index == 10)
          history_index = 0;
      fprintf(history_file, "%s", input);
    }
  }
  fclose(history_file);
  return 0;
}

