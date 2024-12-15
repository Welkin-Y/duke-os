#include <stdlib.h>
#include <stdio.h>

#define _XOPEN_SOURCE 1
#include <ucontext.h>

/*
 * Illustrate use of context save/restore: a key idea behind threads.
 * Save and restore a single context.
 */

int count = 0;
ucontext_t context;

int
main() {
  int i = 0;
  getcontext(&context);

  count += 1;
  i += 1;

  sleep(2);
  printf("%4d %4d\n", count, i);

  setcontext(&context);
}

/*
0000000100000eb0	movl	0x0000017a(%rip),%ecx
0000000100000eb6	addl	$0x00000001,%ecx
0000000100000ebc	movl	%ecx,0x0000016e(%rip)
0000000100000ec2	movl	0xfc(%rbp),%ecx
0000000100000ec5	addl	$0x00000001,%ecx
0000000100000ecb	movl	%ecx,0xfc(%rbp)

with O2:
0000000100000ed5	incl	0x00000155(%rip)
...
0000000100000ef4	movl	$0x00000001,%edx

http://turkeyland.net/projects/overflow/crash.php

 */
