#include <stdio.h>

int main() {
  char input[1024];
  while (1) 
  {
    if (fgets(input, 1024, stdin) == NULL) 
        break;
    printf("%s", input);
  }
  return 0;
}

