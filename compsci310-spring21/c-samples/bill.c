#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <signal.h>
#include <unistd.h>

void siginthandler()
{
  printf("Noooo!  Don't kill me!\n");
}

void signalInit(void)
{
  struct sigaction sigHandler;
  sigemptyset(&sigHandler.sa_mask);
  sigHandler.sa_flags = 0;
  sigHandler.sa_handler = siginthandler;
  sigaction(SIGINT, &sigHandler, NULL);
}

int 
main(int argc, char* argv[])
{
  signalInit();
  while(1)
    sleep(1);
}
