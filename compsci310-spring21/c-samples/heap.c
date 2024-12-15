#include <stdlib.h>
#include <stdio.h>

int 
main()
{
  char* cb = (char*) malloc(14);
  cb[0]='h';
  cb[1]='i';
  cb[2]='!';
  cb[3]='\0';
  printf("%s\n", cb);
  int *ip = (int*)cb;
  printf("0x%x\n", *ip);
  printf("%d\n", *ip);
  free(cb);
  printf("0x%x\n", *ip);
}
