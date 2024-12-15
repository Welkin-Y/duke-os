int val = 0;

int p1(char *sptr) {
  return 1;
}

int p2() {
  char *s;
  int i;
  s = "hello\n";
  i = p1(s);
  return(i);
} 

