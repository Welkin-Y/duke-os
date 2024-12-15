#include <stdio.h>

extern int p1();
extern int p2();

int 
main()
{
  int i;

  i = p2();
  printf("%d\n", i);
}
