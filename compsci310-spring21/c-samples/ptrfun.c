#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h> // needed for mmap

int i = 0;

void sigseghandler()
{
  printf("Not my fault! (0x%x)\n", i);
  exit(1);
}

void signalInit(void)
{
  struct sigaction sigHandler;
  sigemptyset(&sigHandler.sa_mask);
  sigHandler.sa_flags = 0;
  sigHandler.sa_handler = sigseghandler;
  sigaction(SIGSEGV, &sigHandler, NULL);
}

int 
main(int argc, char* argv[])
{
  char* pointer = 0;
  size_t max_bytes = 2*1024*1024;
  char c;
  
  signalInit();

  pointer = (char*) mmap(NULL, max_bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  if (pointer == (char*)-1) {
    perror("mmap failed");
    exit(1);
  }
  printf("Region starts at 0x%lx\n", (unsigned long)pointer); fflush(stdout);

  for (i=0; i<max_bytes; i++)
    c = pointer[i];
  printf("Scanned the whole region, pausing..\n");  fflush(stdout);
  sleep(5);
  for (i=0; i<32*1024*1024; i++)
    c = pointer[i];
}
