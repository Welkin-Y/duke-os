#include <stdio.h>
#include <stdlib.h>

int 
main(int argc, char* argv[], char* envp[])
{
  int i;
  int count = atoi(argv[1]);

  for (i=0; i < count; i++) {
    printf("env %d: %s\n", i, envp[i]);
  }
}
