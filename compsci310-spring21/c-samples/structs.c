#include <stdio.h>
#include <stdlib.h>

struct stuff {
  int i;
  long j;
  char c[2];
};

struct stuff gstuff;

int main() {
  struct stuff sstuff;
  struct stuff *hstuffp = (struct stuff *)malloc(sizeof(struct stuff));

  gstuff.i = 13;
  gstuff.j = 14;
  gstuff.c[0] = 'z';
  gstuff.c[1] = '\0';

  printf("%s\n", gstuff.c);

  sstuff.i = 13;
  sstuff.j = 14;
  sstuff.c[0] = 'z';
  sstuff.c[1] = '\0';

  hstuffp->i = 13;
  hstuffp->j = 14;
  hstuffp->c[0] = 'z';
  hstuffp->c[1] = '\0';
}
