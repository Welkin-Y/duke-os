#include <stdio.h>

/*main(int argc, char** argv)*/
int 
main(int argc, char* argv[])
{
  int i;

  printf("arguments: %d\n", argc);
  for (i=0; i<argc; i++) {
    printf("%d: %s\n", i, argv[i]);
  }
}
