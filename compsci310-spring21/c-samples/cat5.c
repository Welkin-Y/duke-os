#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define BUFSIZE 8192
char buf[BUFSIZE];

int 
main()
{
  size_t count = 5;

  count = read(0, buf, count);
  if (count == -1)  {
    perror("read failed");
    exit(1);
  }
  count = write(1, buf, count);
  if (count == -1) {
    perror("write failed");
    exit(1);
  }
  fprintf(stderr, "\n%zd bytes moved\n", count);
}
