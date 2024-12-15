#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 8192
char buf[BUFSIZE];

int ifd[2] = {0, 0};	/* ifd[0] is read, ifd[1] is write */

int
main(int argc, char *argv[])
{
  int status;
  int rc;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <program to exec>\n", argv[0]);
    exit(1);
  }

  pipe(ifd);

  rc = fork();
  if (rc < 0) {
    perror("fork failed: ");
    exit(1);
  } else if (rc == 0) {
    close(0);
    close(ifd[1]);
    dup2(ifd[0],0);
    argv++;
    execve(argv[0], argv, 0);
    perror("execve");
  } else {
    size_t count = 5;
    close(ifd[0]);
    count = read(0, buf, count);
    if (count == -1)  {
      perror("parent read from stdin failed");
      exit(1);
    }
    count = write(ifd[1], buf, count);
    if (count == -1) {
      perror("parent write to child failed");
      exit(1);
    }
    if (waitpid(rc, &status, 0) == -1)
      perror("waitpid");
    printf("child %d exited with status %d\n", rc,
	   WEXITSTATUS(status));
  }
}
