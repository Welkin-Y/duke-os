#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
  fork();
  fork();
  fork();
  fork();
  fork();
  fork();
  printf("Process %d exiting.\n", getpid());
}
