#include <stdio.h>
#include <string.h>

int main() {
  // Uncomment this block to pass the first stage
  printf("$ ");
  fflush(stdout);
  char input[100];
  fgets(input, 100, stdin);

  // Wait for user input
  input[strlen(input)-1]='\0';
  printf("%s: command not found\n", input);
  return 0;
}

