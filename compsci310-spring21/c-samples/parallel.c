#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

/*
 * Timer module --------------------------
 */

long starttime = 0;

long
timer_mark()
{
        struct timeval t_time;
        struct timeval *tvp;
        struct rusage myusage;

	tvp = &t_time;
	gettimeofday(tvp, 0);

	/*       
        if (getrusage(RUSAGE_SELF, &myusage) == -1) {
	  perror("getrusage failed");
	  exit(1);
	}
	  
	tvp = &(myusage.ru_utime);
	 */
        double ms = ((double)tvp->tv_sec * 1000) +
	  ((double)(tvp->tv_usec) * 1.0e-3);
	long msl = (long)ms;
	return(msl);
}

long
timer_elapsed()
{
  return (timer_mark() - starttime);
}
  
void
timer_reset()
{
  starttime = timer_mark();
}

/*
 * End of timer module --------------------------
 */



int count = 0;
int number = 0;
void dofork();

#define MAXPROCS 500
int procs[MAXPROCS];

#define BUSYWORK 1000000000

volatile long zot;

void
child()
{
  long z;

  for (z = 1; z <= BUSYWORK; z++) {
    zot = BUSYWORK/z;
  }

  /*
    printf("Child %d is done.\n", (int) getpid());
  */
  exit(0);
}

void
parent(int childpid)
{
  /*
    printf("Forked child %d for slot %d.\n", childpid, number);
  */
  procs[number] = childpid;
}

void
dofork()
{
  int rc = fork();
  if (rc < 0) {
    perror("fork failed: ");
    exit(1);
  } else if (rc == 0) {
    child();
  } else {
    parent(rc);
  }
}

void runem(int n)
{
  int status;

  for (number = 0; number < n; number++) {
    dofork();
  }

  for (number = 0; number < n; number++) {
    waitpid(procs[number], &status, 0);
    /*    printf("Child %d exited with status %d.\n", procs[number],
	   WEXITSTATUS(status));
    */
  }
}

int
main(int argc, char *argv[])
{
  int i;
  long z;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <numprocs>\n", argv[0]);
    exit(1);
  }
  count = atoi(argv[1]);
  if (count < 0 || count > MAXPROCS) {
    fprintf(stderr, "Too many procs, %d not between 0 and %d.\n", count, MAXPROCS);
    exit(1);
  }

  for (i = 1; i <= count; i++) {
    timer_reset();
    runem(i);

    printf("%d, %ld\n", i, timer_elapsed());
  }
}

