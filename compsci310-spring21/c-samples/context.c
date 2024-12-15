#include <stdlib.h>
#include <stdio.h>

#define _XOPEN_SOURCE 1
#include <ucontext.h>

/*
 * Illustrate use of context save/restore: a key idea behind threads.
 * This variant is a little tricky because there is only one stack.
 * Save (get and store in memory) a pair of contexts (even and odd),
 * each at the start of a straight-line code sequence.  Then load context
 * (set from memory) and run them over and over again forever, handing
 * off to one another.
 */

int count = 0;
int set = 0;
ucontext_t contexts[2];

void proc()
{
  int i = 0;
  if (!set) {
    getcontext(&contexts[count]);
  }
  printf("%4d %4d\n", count, i);
  count += 1;
  i += 1;
  sleep(1);
  if (set) {
    setcontext(&contexts[count&0x1]);
  }
}

int 
main()
{
  proc();
  proc();
  set = 1;
  proc();
}
