#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define BUFSIZE 8192
char buf[BUFSIZE];

#define CHUNKSIZE 200

int 
main(int argc, char* argv[])
{
  size_t count = CHUNKSIZE;
  int delay = 1;

  if (argc > 1) 
    delay = atoi(argv[1]);

  while(count > 0) {
    count = read(0, buf, CHUNKSIZE);
    if (count == -1)  {
      perror("read failed");
      exit(1);
    }
    count = write(1, buf, count);
    if (count == -1) {
      perror("write failed");
      exit(1);
    }
    sleep(delay);
  }
}
