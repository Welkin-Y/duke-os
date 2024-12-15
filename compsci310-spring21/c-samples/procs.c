#include <string.h>
#include <assert.h>

int val;

int p1(char *sptr) {
  int len = strlen(sptr);
  assert(!val);
  return len;
}

int p2() {
  char *s;
  int i;
  s = "hello\n";
  i = p1(s);
  return(i);
} 

