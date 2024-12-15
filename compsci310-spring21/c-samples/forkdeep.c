#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int count = 0;
int level = 0;
void dofork();

void
tablevel()
{
  int i;
  for (i = 0; i < level; i++)
    printf("        ");
}

void
child()
{
  level++;
  tablevel();
  printf("%8d\n", (int) getpid());
  if (level < count)
    dofork();
  else if (level == count)
    sleep(3);
}

void
parent(int childpid)
{
  int status;
  tablevel();
  printf("%8d->%6d\n", (int) getpid(), childpid);
  waitpid(childpid, &status, 0);
}

void
dofork()
{
  int rc = fork();
  if (rc < 0) {
    perror("fork failed: ");
    exit(1);
  } else if (rc == 0) {
    child();
  } else {
    parent(rc);
  }
}

int
main(int argc, char *argv[])
{
  count = atoi(argv[1]);
  dofork();
  tablevel();
  printf("%8d\n", (int) getpid());
}

