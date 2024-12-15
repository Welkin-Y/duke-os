#include <stdlib.h>
#include <stdio.h>

int 
main()
{
  long* lb = (long*) malloc(14);
  printf("0x%lx first pointer (to long)\n", (unsigned long)lb);  
  lb+=1;
  printf("0x%lx incremented by one!\n", (unsigned long)lb);  
  /*
   * If you do arithmetic on a typed pointer, C may surprise you.
   *
   * In this case, the virtual address in lb is incremented by 4 or 8 
   * (sizeof long) and not by 1.  In essence, C treats the addition to
   * the pointer as addition to an array index.  So adding "one" to 
   * lb means: "since lb points to the first long in a block of longs,
   * increment it so it points to the second long in the block of longs".
   *
   * If you really want to do pointer offset arithmetic, always do
   * your arithmetic on unsigned long (or void*), and then cast the
   * result back to the pointer type you want.
   */
  void* vp = malloc(14);
  printf("%p second pointer (void*)\n", vp);
  vp+=1;
  printf("%p incremented by one\n", vp);
}
