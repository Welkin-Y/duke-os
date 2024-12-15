#include <stdio.h>
#include <stdlib.h>

#define SIZE 10
char buffer[SIZE];

void
fill(char* buf, int i)
{
  for(int j=0; j<i-1; j++)
    buf[j] = (char)(((int)'a')+j);
  return;
}

int
main()
{
  fill(buffer, SIZE);
  printf("%s\n", buffer);
}
