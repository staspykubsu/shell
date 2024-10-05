#include <stdio.h>
#include <string.h>

int main() {
  char input[1024];
  while (1) 
  {
    if (fgets(input, 1024, stdin) == NULL)
        break;
    if (strcmp(input, "exit\n") == 0 || strcmp(input, "\\q\n") == 0)
        break;
    printf("%s", input);
  }
  return 0;
}

