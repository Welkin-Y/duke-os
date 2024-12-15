#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

int 
main()
{
  char* cb = (char*) malloc(14);
  strcpy(cb, "hi!");
  free(cb);
  /*
   * Dangling reference!
   */
  printf("%s\n", cb);
  int *ip = (int*)cb;
  printf("0x%x\n", *ip);

  char* cb2 = (char*) malloc(14);
  printf("%s\n", cb2);  
}
