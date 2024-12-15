#include <stdio.h>
#include <stdlib.h>

int 
main(int argc, char* argv[], char* envp[])
{
  int i;
  int count;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <count>\n", argv[0]);
    exit(1);
  }
  count = atoi(argv[1]);

  for (i=0; i < count; i++) {
    if (envp == 0) {
      printf("env %d: nothing!\n", i);
      exit(1);
    }
    else if (envp[i] == 0) {
      printf("env %d: null!\n", i);
      exit(1);
    } else
      printf("env %d: %s\n", i, envp[i]);
  }
}
