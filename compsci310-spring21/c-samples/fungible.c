#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *s = "It was the best of times, it was the worst of times, \nit was the age of wisdom, it was the age of foolishness, \nit was the epoch of belief, it was the epoch of incredulity, \nit was the season of Light, it was the season of Darkness, \nit was the spring of hope, it was the winter of despair, \nwe had everything before us, we had nothing before us...\n";

int 
main()
{
  size_t bytecount;
  unsigned long *lp;
  int i,j, k;

  /*
   * The value of a string constant is a pointer: an address where the string resides.
   * The string value is in the executable (ELF), along with every other string constant in the program.
   * Program launch pulls it from the executable and copies it into a readonly data segment in the VAS.
   */
  printf("%s\n", s);
  printf("That string constant resides at address %p.  Pointers print in hex format!\n", s);

  /* When you're counting bytes you should use size_t (an unsigned long).  Print it as %zu.  Just do. */
  bytecount = strlen(s);
  
  /* But if you to make your own string you have to allocate memory for it, as a char array or malloc up a block. */
  char* p = (char*) malloc(bytecount);
  if (p == NULL) {
    perror("Always check error returns from malloc: it returns null (0) if something goes wrong.");
    exit(1);
  }

  printf("That string is %zu bytes.\n\n", bytecount);

  printf("%p ", p); printf("Here's a heap block for a copy of the string.  Pointers print in hex format!\n");
  printf("%lu ", (unsigned long)p); printf("You can use decimal (long unsigned) if you insist, but it's not as pretty.\n");
  printf("%ld ", (long)p); printf("It might or might not look different if you leave it signed.\n\n");

  /*
   * Copying a string: for safety, always use strncpy with a size bound, instead of strcpy.
   * Watch the order of arguments.  memcpy and strcpy order like assignments: target <- source.
   * Always check the return!  (OK I got lazy.)
   */
  strncpy(p, s, bytecount);

  /* Memory is fungible: let's treat that string like an array of longs. */
  lp = (unsigned long*)p;

  /*
   * We can look at the characters 8 at a time (or 4 at a time) in hex.
   * That's what the ASCII character representation looks like, printed as numbers.
   * One hex digit = 4 bits, two hex digits = one byte.
   */
  printf("0x%lx ", lp[0]); printf("Here are some characters in hex (ASCII coded).\n");
  printf("0x%lx ", lp[1]); printf("Here are some more characters.\n");
  printf("0x%lx ", lp[2]); printf("And some more.\n\n");

  printf("%lu ", lp[0]); printf("Here are the same characters in decimal.  Not so easy to see.\n");
  printf("%lu ", lp[1]); printf("And some more characters.\n");
  printf("%lu ", lp[2]); printf("And some more.\n\n");

  /* Here's how to get a pointer to an entry in an array. */
  printf("%p ", &lp[0]); printf("Here is the address for the first nibble.\n");
  printf("%p ", &lp[1]); printf("And the second nibble: one long's worth at a time!\n");
  printf("%p ", &lp[2]); printf("And the third.\n\n");

  /* You can do the same thing with typed pointer arithmetic: arrays "are" pointers. */

  printf("%p ", lp); printf("Here is the address for the first nibble again.\n");
  printf("%p ", lp++); printf("And the second nibble again.\n");
  printf("%p ", lp++); printf("And the third.\n\n");
  lp = (unsigned long*)p;
  
  /*
   * A char* is a pointer to a character, could be a longer string.
   * So if we want to print a long as a string, we need a pointer to it, and cast to char*.
   * We have to be careful printing strings, if what we want isn't null-terminated.
   * So tell printf how many bytes we want to print at a time: sizeof(*lp) == 8 (or 4).
   * Note that we say *lp there because we want the size of thing the pointer points at, not the pointer itself.
   * (Eventhough it's the same size, the compiler will complain.)
   * This is a weird printf format that says don't print more than i characters, where i is an integer.
   */
  i = (int)sizeof(*lp);
  printf("Now let's print those nibbles as groups of characters, a long's worth at a time:\n");
  printf("%.*s \n", i, (char*)&lp[0]);
  printf("%.*s \n", i, (char*)&lp[1]);
  printf("%.*s \n\n", i, (char*)&lp[2]);

  printf("In fact we can print the whole string that way by walking it as an array of longs:\n\n");
  for(j=0, k=0; k<(int)bytecount; j++, k+=i) {
    printf("%.*s", i, (char*)&lp[j]);
  }

  printf("\nWhat if we increment each long by one first?\n");
  printf("Still recognizable but it goobers some of the chars:\n\n");
  for(j=0, k=0; k<(int)bytecount; j++, k+=i) {
    lp[j]++;
  }  
  for(j=0, k=0; k<(int)bytecount; j++, k+=i) {
    printf("%.*s", i, (char*)&lp[j]);
  }


}
