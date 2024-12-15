#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

int 
main()
{
  char* cb = (char*) malloc(14);
  strcpy(cb, "hi!");
  unsigned long ptr = (unsigned long)cb;
  ptr = ptr + 2;
  cb = (char*)ptr;
  printf("%s\n", cb);  
  free(cb);
}
