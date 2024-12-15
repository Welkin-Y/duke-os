#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char *argv[], char *envp[])
{
  int status;
  int rc = fork();
  if (rc < 0) {
    perror("fork failed: ");
    exit(1);
  } else if (rc == 0) {
    argv++;
    execve(argv[0], argv, envp);
  } else {
    waitpid(rc, &status, 0);
    printf("child %d exited with status %d\n", rc,
	   WEXITSTATUS(status));
  }
}
